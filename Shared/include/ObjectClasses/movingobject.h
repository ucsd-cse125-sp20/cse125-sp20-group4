#pragma once
#include "object.h"
#include <glm/glm.hpp>
#define SERVER_TICK 50

class MovingObject : public Object {
private:
    glm::vec3 velocity;
public:
    MovingObject(std::string id);
    MovingObject(std::string id, float x, float y, float z);
    MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ);
    void setVelocityX(float velX);
    void setVelocityY(float velY);
    void setVelocityZ(float velZ);
    void setVelocity(float velX, float velY, float velZ);
    void setVelocity(glm::vec3 velocity);
    float getVelocityX() const;
    float getVelocityY() const;
    float getVelocityZ() const;
    glm::vec3 getVelocity() const;
    float getNextPositionX() const;
    float getNextPositionY() const;
    float getNextPositionZ() const;
    //(de)serialize functions
    virtual std::string serialize() override;
};