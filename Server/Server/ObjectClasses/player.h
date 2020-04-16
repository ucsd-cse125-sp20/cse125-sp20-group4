#pragma once
#include "movingobject.h"

class Player : public MovingObject {
private:
    //inventory??
    int disregard;

public:
    Player(int id);
    Player(int id, int x, int y, int z);
    Player(int id, int x, int y, int z, float hor, float ver);
    Player(int id, int x, int y, int z, float hor, float ver, float velX, float velY, float velZ);
};