//
// Created by Lucas on 4/2/2023.
//

#ifndef FRETTREE_FRETBOARD_H
#define FRETTREE_FRETBOARD_H

#include <vector>
#include "Acoustics.h"

enum NoteNames {
        C = 0,
        Db = 1,
        D = 2,
        Eb = 3,
        E = 4,
        F = 5,
        Gb = 6,
        G = 7,
        Ab = 8,
        A = 9,
        Bb = 10,
        B = 11
    };

enum Octaves {
        oct0 = 0,
        oct1 = 1,
        oct2 = 2,
        oct3 = 3,
        oct4 = 4,
        oct5 = 5,
        oct6 = 6,
        oct7 = 7,
        oct8 = 8,
    };

double frequencies[12][9] = {
        {16.35, 32.70, 65.41, 130.81, 261.63, 523.25, 1046.50, 2093.00, 4186.01},
        {17.32, 34.65, 69.30, 138.59, 277.18, 554.37, 1108.73, 2217.46, 4434.92},
        {18.35, 36.71, 73.42, 146.83, 293.66, 587.33, 1174.66, 2349.32, 4698.63},
        {19.45, 38.89, 77.78, 155.56, 311.13, 622.25, 1244.51, 2489.02, 4978.03},
        {20.60, 41.20, 82.41, 164.81, 329.63, 659.26, 1318.51, 2637.02, 5274.04},
        {21.83, 43.65, 87.31, 174.61, 349.23, 698.46, 1396.91, 2793.83, 5587.65},
        {23.12, 46.25, 92.50, 185.00, 369.99, 739.99, 1479.98, 2959.96, 5919.91},
        {24.50, 49.00, 98.00, 196.00, 392.00, 783.99, 1567.98, 3135.96, 6271.93},
        {25.96, 51.91, 103.83, 207.65, 415.30, 830.61, 1661.22, 3322.44, 6644.88},
        {27.50, 55.00, 110.00, 220.00, 440.00, 880.00, 1760.00, 3520.00, 7040.00},
        {29.14, 58.27, 116.54, 233.08, 466.16, 932.33, 1864.66, 3729.31, 7458.62},
        {30.87, 61.74, 123.47, 246.94, 493.88, 987.77, 1975.53, 3951.07, 7902.13}
};



struct NoteData {
    int noteName;
    int octave;
    double frequency;
};

NoteData* createNoteData(int name, int octave) {
    auto* noteData = new NoteData();
    noteData->noteName = name;
    noteData->octave = octave;
    noteData->frequency = frequencies[name][octave];
    return noteData;

}

struct Fret {
    Fret() = default;
    Fret(int row, int column, NoteData* noteData) : row{row}, column{column} {
        this->noteData = noteData;
    };

    int row;
    int column;

    NoteData* noteData;

    bool visited = false;

    Fret* up = nullptr;
    Fret* upLeft = nullptr;
    Fret* upRight = nullptr;
    Fret* left = nullptr;
    Fret* right = nullptr;
    Fret* downLeft = nullptr;
    Fret* downRight = nullptr;
    Fret* down = nullptr;

    friend std::ostream& operator<<(std::ostream& stream, const Fret* fret) {
        stream << '(' << fret->row << ", " << fret->column << ')';
        return stream;
    }

};


template <int rows, int cols>
struct FretBoard {
    FretBoard() = default;
    FretBoard(NoteData* openStrings[cols]) {
        this->numRows = rows;
        this->numColumns = cols;
        fillNoteMatrix(openStrings);
    }

    int numRows = 0;
    int numColumns = 0;
    NoteData* noteDataMatrix[rows][cols];

    void fillNoteMatrix(NoteData* openStrings[cols]) {
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                // first column round, fill with values from openStrings
                if (r == 0) {
                    noteDataMatrix[r][c] = openStrings[c];
                }
                else {
                    // if the previous note in this row was a B, go to the next octave
                    if ((noteDataMatrix[r - 1][c]->noteName + 1) % 12 == 0) {
                        noteDataMatrix[r][c] = createNoteData((noteDataMatrix[r - 1][c]->noteName + 1) % 12,
                                                              noteDataMatrix[r - 1][c]->octave + 1);
                    }
                    else {
                        noteDataMatrix[r][c] = createNoteData((noteDataMatrix[r - 1][c]->noteName + 1) % 12,
                                                        noteDataMatrix[r - 1][c]->octave);
                    }
                }
            }
        }
    }

    void printFretboardData() {
        for (int r = 0; r < rows; r++) {
            std::cout << "-------------------------------------" << '\n';
            for (int c = 0; c < cols; c++) {
                std::cout << "ROW:" << r << '-' << "COL:" << c << "  (" << noteDataMatrix[r][c]->noteName << ", "
                          << noteDataMatrix[r][c]->octave << ")" << '\n';
            }
        }
    }

    Fret* constructFret(int r, int c, std::vector<std::vector<Fret*>>& visited) {

        if ((r >= rows || c >= cols) || (r < 0 || c < 0)) {return nullptr;}

        if (visited[r][c]) {return visited[r][c];}

        Fret* newFret = new Fret(r, c, noteDataMatrix[r][c]);
        visited[r][c] = newFret;

        newFret->up = constructFret(r - 1, c, visited);
        newFret->down = constructFret(r + 1, c, visited);

        newFret->left = constructFret(r, c - 1, visited);
        newFret->right = constructFret(r, c + 1, visited);

        newFret->upLeft = constructFret(r - 1, c - 1, visited);
        newFret->upRight = constructFret(r - 1, c + 1, visited);

        newFret->downLeft = constructFret(r + 1, c - 1, visited);
        newFret->downRight = constructFret(r + 1, c + 1, visited);


        return newFret;






//            }
//        }

    }

};


#endif //FRETTREE_FRETBOARD_H
