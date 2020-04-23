#pragma once
#include "object.h"
#define SERVER_TICK 50

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
    void setVelocity(float velX, float velY, float velZ);
    float getVelocityX();
    float getVelocityY();
    float getVelocityZ();
    int getNextPositionX();
    int getNextPositionY();
    int getNextPositionZ();
    //(de)serialize functions
    virtual std::string serialize() override;
};