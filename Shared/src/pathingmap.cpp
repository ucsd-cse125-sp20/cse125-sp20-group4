#include "pathingmap.h"

PathingMap::PathingMap(int dimX, int dimY) {
    // always leave border of boundary-type pathing
    this->dimX = (dimX < MAX_DIM - 2) ? dimX : MAX_DIM - 2;
    this->dimY = (dimY < MAX_DIM - 2) ? dimY : MAX_DIM - 2;
    for (int i = 0; i < this->dimX; i++) {
        for (int j = 0; j < this->dimX; j++) {
            map[i + 1][j + 1] = EMPTY;
        }
    }
}

bool PathingMap::parsePathing(std::string fromString) {
    // TODO implement parsing a pathing map from a string/file
    return false;
}

// TODO maybe bounds checking...
char PathingMap::getPathing(int x, int y) {
    return map[x][y];
}

void PathingMap::setPathing(int x, int y, char p) {
    map[x][y] = p;
}
