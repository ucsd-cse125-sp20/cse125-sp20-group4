#include "pathingmap.h"

#include <queue>

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

// returns queue of coordinates to move object to
std::stack<struct MapCoord> PathingMap::getPath(int srcX, int srcY, int dstX, int dstY) {
    struct Node {
        bool visited;
        struct MapCoord prev;
    };

    struct Node nodes[MAX_DIM][MAX_DIM] = {};

    // do basic BFS
    std::queue<struct MapCoord> searchQueue = std::queue<struct MapCoord>();
    // push source
    nodes[srcX][srcY].visited = true;
    nodes[srcX][srcY].prev = { -1, -1 };
    searchQueue.push({ srcX, srcY });

    // iterate until empty
    while (!nodes[dstX][dstY].visited && !searchQueue.empty()) {
        struct MapCoord curr = searchQueue.front();
        searchQueue.pop();
        // no bounds checking since map has unpathable border. just don't start a pathing search from (0, 0)
        for (int i = -1; i <= 1; i += 2) {
            int x = curr.x + i;
            int y = curr.y;
            if (!nodes[x][y].visited && map[x][y] == EMPTY) {
                nodes[x][y].visited = true;
                nodes[x][y].prev = curr;
                searchQueue.push({ x, y });
            }
        }
        for (int i = -1; i <= 1; i += 2) {
            int x = curr.x;
            int y = curr.y + i;
            if (!nodes[x][y].visited && map[x][y] == EMPTY) {
                nodes[x][y].visited = true;
                nodes[x][y].prev = curr;
                searchQueue.push({ x, y });
            }
        }
    }

    std::stack<struct MapCoord> cmdStack = std::stack<struct MapCoord>();
    // no path so empty cmds
    if (!nodes[dstX][dstY].visited) {
        return cmdStack;
    }
    // otherwise walk back from dest to get path
    struct MapCoord walkBack = { dstX, dstY };
    while (walkBack.x != -1) {
        cmdStack.push(walkBack);
        walkBack = nodes[walkBack.x][walkBack.y].prev;
    }
    return cmdStack;
}
