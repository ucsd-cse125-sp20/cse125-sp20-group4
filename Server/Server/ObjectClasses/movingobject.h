#pragma once
#include "object.h"

class MovingObject : public Object {
private:
    float velocityX, velocityY, velocityZ;
public:
    MovingObject(int id);
    MovingObject(int id, int x, int y, int z);
    MovingObject(int id, int x, int y, int z, float hor, float ver);
    MovingObject(int id, int x, int y, int z, float hor, float ver, float velX, float velY, float velZ);
    void setVelocityX(float velX);
    void setVelocityY(float velY);
    void setVelocityZ(float velZ);
    void setVelocity(float velX, velY, velZ);
    float getVelocityX();
    float getVelocityY();
    float getVelocityZ();
};