#include "movingobject.h"

MovingObject::MovingObject(int id) : Object(id) {
    setVelocity(0.0f, 0.0f, 0.0f);
}

MovingObject::MovingObject(int id, int x, int y, int z) : Object(id, x, y, z) {
    setVelocity(0.0f, 0.0f, 0.0f);
}

MovingObject::MovingObject(int id, int x, int y, int z, float hor, float ver) : Object(id, x, y, z, hor, ver) {
    setVelocity(0.0f, 0.0f, 0.0f);
}

MovingObject::MovingObject(int id, int x, int y, int z, float hor, float ver, float velX, float velY, float velZ) : Object(id, x, y, z, hor, ver) {
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

float MovingObject::getVelocityX() {
    return this->velocityX;
}

float MovingObject::getVelocityY() {
    return this->velocityY;
}

float MovingObject::getVelocityZ() {
    return this->velocityZ;
}