#pragma once
#include "movingobject.h"

class Player : public MovingObject {
private:
    //inventory??
    int disregard;

public:
    Player(int id);
    Player(int id, int x, int y, int z);
    Player(int id, int x, int y, int z, float orientationX, float orientationY, float orientationZ);
    Player(int id, int x, int y, int z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ);
    //(de)serialize functions
    virtual std::string serialize() override;
};