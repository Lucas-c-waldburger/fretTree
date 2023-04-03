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



int main() {
    std::pair<int, int> openStrings[6] = {
            std::make_pair<int, int>(E, oct2),
            std::make_pair<int, int>(A, oct2),
            std::make_pair<int, int>(D, oct3),
            std::make_pair<int, int>(G, oct3),
            std::make_pair<int, int>(B, oct3),
            std::make_pair<int, int>(E, oct4),
    };


    auto fretBoard = FretBoard<13, 6>(openStrings);

//    fretBoard.printFretboardData();

    std::vector<std::vector<Fret*>> visited(fretBoard.numRows, std::vector<Fret*>(fretBoard.numColumns));

    Fret* root = fretBoard.constructFret(0, 0, visited);

    printFrets(root);

    return 0;
}
