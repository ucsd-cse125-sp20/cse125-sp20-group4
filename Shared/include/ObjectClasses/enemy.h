#pragma once
#include "movingobject.h"
#include "pathingmap.h"
#include "ObjectClasses/Useable/useable.h"
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

    const std::string TAG = "Enemy";
    const std::string& getTag();

    // sets the velocity from top command of cmdStack
    void setVelocityFromCmd();

    bool isEnemy() const;

    //(de)serialize functions
    virtual std::string serialize() const override;
    virtual std::shared_ptr<Object> clone() const;


    static Enemy& deserialize(std::string serial);
    ItemType weakness;
};