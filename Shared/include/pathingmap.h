#pragma once

#include <stack>
#include <string>

#define MAX_DIM 1024
#define BOUND   0x0 // pathing block defining map bounds
#define EMPTY   0x1 // no pathing block
#define NO_WALK 0x2 // cannot path through area

struct MapCoord {
    int x;
    int y;
};

// grid describing map pathing for AI

class PathingMap {
private:
    // because i'm too lazy to use std::vectors
    char map[MAX_DIM][MAX_DIM] = { 0 };
    int dimX;
    int dimY;
public:
    PathingMap(int, int);
    bool parsePathing(std::string);
    char getPathing(int, int);
    void setPathing(int, int, char);
    // TODO implement a function that converts map coord to 3d coordinates  
    std::stack<MapCoord> getPath(int, int, int, int);
};