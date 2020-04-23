#pragma once
#include "movingobject.h"

class Player : public MovingObject {
private:
    //inventory??
    int disregard;

public:
    Player(int id);
    Player(int id, float x, float y, float z);
    Player(int id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    Player(int id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ);
    //(de)serialize functions
    virtual std::string serialize() override;
};