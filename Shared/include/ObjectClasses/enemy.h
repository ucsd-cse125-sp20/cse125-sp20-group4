#pragma once
#include "movingobject.h"
#include "pathingmap.h"

#include <stack>

class Enemy : public MovingObject {
private:
    float baseSpeed;
    std::stack<struct MapCoord> cmdStack;

public:
    Enemy(const Enemy& player);
    Enemy(std::string id);
    Enemy(std::string id, float x, float y, float z);
    Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ);

    // sets the velocity from top command of cmdStack
    void setVelocityFromCmd();

    bool isEnemy() const;

    //(de)serialize functions
    virtual std::string serialize() const override;

    static Enemy& deserialize(std::string serial);
};