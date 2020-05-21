#pragma once
#include "movingobject.h"

class Player : public MovingObject {
private:
    //inventory??
    int disregard;

public:
    Player(const Player& player);
    Player(std::string id);
    Player(std::string id, float x, float y, float z);
    Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length);
    Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length, float velX, float velY, float velZ);
    //(de)serialize functions
    virtual std::string serialize() const override;

    static Player& deserialize(std::string serial);
};