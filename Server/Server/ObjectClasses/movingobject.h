#pragma once
#include "object.h"
#include <glm/glm.hpp>

class MovingObject : public Object {
private:
    float velocityX, velocityY, velocityZ;
public:
    MovingObject(int id);
    MovingObject(int id, float x, float y, float z);
    MovingObject(int id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    MovingObject(int id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ);
    void setVelocityX(float velX);
    void setVelocityY(float velY);
    void setVelocityZ(float velZ);
    void setVelocity(float velX, float velY, float velZ);
    float getVelocityX();
    float getVelocityY();
    float getVelocityZ();
    //(de)serialize functions
    virtual std::string serialize() override;
};