#include <iostream>
#include "Fretboard.h"
#include "Acoustics.h"
#include <memory>
#include <random>

struct Random {
    Random() {
        distrib.param(std::uniform_int_distribution<int>::param_type(0, 7));
        mt.seed(rd());
    }

    std::mt19937 mt;
    std::random_device rd;
    std::uniform_int_distribution<> distrib;

    int getRand() {
        return distrib(mt);
    }
};


std::vector<Fret*> getRandomNextSequence(Fret* fret) {
    Random random = Random();

    std::vector<Fret*> fretNexts = {fret->up, fret->upLeft, fret->upRight, fret->left, fret->right,
                                    fret->downLeft, fret->downRight, fret->down};

    for (int i = 0; i < fretNexts.size(); i++) {
        int randIndex = random.getRand();
        std::swap(fretNexts[i], fretNexts[randIndex]);
    }

    return fretNexts;
}


void printFrets(Fret* root) {
    std::vector<Fret*> nexts = getRandomNextSequence(root);

    std::cout << root << " - " << root->noteData->noteName << " > " << root->noteData->octave << std::endl;
    root->visited = true;

    for (Fret* next : nexts) {
        if (next != nullptr && !next->visited) {
            printFrets(next);
        }
    }
}

void findChord(Fret* root, std::vector<int>& chordNotes, std::vector<int>& availableStrings) {
    if (availableStrings.empty()) {return;}
//    std::cout << "current notename: " << root->noteData->noteName << ", " << "current string: " << root->column << '\n';

    for (int i = 0; i < availableStrings.size(); i++) {
        if (root->noteData->noteName == chordNotes[i]) {
//            std::cout << "found note!\n";
            for (int j = 0; j < availableStrings.size(); j++) {
                if (root->column == availableStrings[j]) {
//                    std::cout << "found string!\n";
                    std::cout << '(' << root->row << ", " << root->column << ") - {NAME:" << root->noteData->noteName
                              << ", OCTAVE:" << root->noteData->octave << "}\n";

                    chordNotes.erase(chordNotes.begin() + i);
                    availableStrings.erase(availableStrings.begin() + j);

//                    std::cout << "available notes: ";
//                    for (int chordNote: chordNotes) {
//                        std::cout << chordNote << ", ";
//                    }
//                    std::cout << '\n';
//                    std::cout << "available strings: ";
//                    for (int string: availableStrings) {
//                        std::cout << string << ", ";
//                    }
//                    std::cout << '\n';
                }
            }
        }
    }

    std::vector<Fret*> nexts = getRandomNextSequence(root);
    for (Fret* next : nexts) {
        if (next != nullptr) {
            findChord(next, chordNotes, availableStrings);
        }
    }


}


int main() {
    NoteData* openStrings[6] = {
            createNoteData(E, oct2),
            createNoteData(A, oct2),
            createNoteData(D, oct3),
            createNoteData(G, oct3),
            createNoteData(B, oct3),
            createNoteData(E, oct4),
    };


    auto fretBoard = FretBoard<13, 6>(openStrings);

//    fretBoard.printFretboardData();

    std::vector<std::vector<Fret*>> visited(fretBoard.numRows, std::vector<Fret*>(fretBoard.numColumns));

    Fret* root = fretBoard.constructFret(0, 0, visited);

    std::vector<int> chordNotes = {C, E, G, B, D, C};
    std::vector<int> availableStrings; for (int i = 0; i < fretBoard.numColumns; i++) {availableStrings.push_back(i);}
    findChord(root, chordNotes, availableStrings);

    return 0;
}
