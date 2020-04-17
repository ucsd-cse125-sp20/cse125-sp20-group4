#include "movingobject.h"

MovingObject::MovingObject(int id) : Object(id) {
    setVelocity(0.0f, 0.0f, 0.0f);
}

MovingObject::MovingObject(int id, int x, int y, int z) : Object(id, x, y, z) {
    setVelocity(0.0f, 0.0f, 0.0f);
}

MovingObject::MovingObject(int id, int x, int y, int z, float orientationY, float orientationX) : Object(id, x, y, z, orientationY, orientationX) {
    setVelocity(0.0f, 0.0f, 0.0f);
}

MovingObject::MovingObject(int id, int x, int y, int z, float orientationY, float orientationX, float velX, float velY, float velZ) : Object(id, x, y, z, orientationY, orientationX) {
    setVelocity(velX, velY, velZ);
}

void MovingObject::setVelocityX(float velX) {
    this->velocityX = velX;
}

void MovingObject::setVelocityY(float velY) {
    this->velocityY = velY;
}

void MovingObject::setVelocityZ(float velZ) {
    this->velocityZ = velZ;
}

void MovingObject::setVelocity(float velX, float velY, float velZ) {
    setVelocityX(velX);
    setVelocityY(velY);
    setVelocityZ(velZ);
}

float MovingObject::getVelocityX() {
    return this->velocityX;
}

float MovingObject::getVelocityY() {
    return this->velocityY;
}

float MovingObject::getVelocityZ() {
    return this->velocityZ;
}

//(de)serialize functions
std::string MovingObject::serialize() {
    std::string res = Object::serialize() + ","
        + std::to_string(velocityX) + ","
        + std::to_string(velocityY) + ","
        + std::to_string(velocityZ);
    return res;
}