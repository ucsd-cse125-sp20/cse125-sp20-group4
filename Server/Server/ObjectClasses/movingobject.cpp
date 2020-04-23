#include "movingobject.h"
#include "Shared/logger.h""

MovingObject::MovingObject(int id) : MovingObject(id, 0.0f, 0.0f, 0.0f,) {}

MovingObject::MovingObject(int id, int x, int y, int z) : MovingObject(id, x, y, z, 0.0f, 0.0f) {}

MovingObject::MovingObject(int id, int x, int y, int z, float orientationX, float orientationY) : MovingObject(id, x, y, z, orientationX, orientationY, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(int id, int x, int y, int z, float orientationX, float orientationY, float velX, float velY, float velZ) : Object(id, x, y, z, orientationX, orientationY) {
    auto log = logger();
    setVelocity(velX, velY, velZ);
    log->trace("Creating MovingObject with id {}, position ({}, {}, {}), orientation ({}, {}), velocity ({}, {}, {})", id, x, y, z, orientationX, orientationY, velX, velY, velZ);

}

void MovingObject::setVelocityX(float velX) {
    auto log = logger();
    this->velocityX = velX;
    log->trace("Setting velocityX of MovingObject with id {}, with value {}", this->getId(), velX);
}

void MovingObject::setVelocityY(float velY) {
    auto log = logger();
    this->velocityY = velY;
    log->trace("Setting velocityY of MovingObject with id {}, with value {}", this->getId(), velY);
}

void MovingObject::setVelocityZ(float velZ) {
    auto log = logger();
    this->velocityZ = velZ;
    log->trace("Setting velocityZ of MovingObject with id {}, with value {}", this->getId(), velZ);
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

int MovingObject::getNextPositionX() {
    return getPositionX() + getVelocityX() * SERVER_TICK;
}

int MovingObject::getNextPositionY() {
    return getPositionY() + getVelocityY() * SERVER_TICK;
}

int MovingObject::getNextPositionZ() {
    return getPositionZ() + getVelocityZ() * SERVER_TICK;
}

//(de)serialize functions
std::string MovingObject::serialize() {
    auto log = logger();
    std::string res = Object::serialize() + ","
        + std::to_string(velocityX) + ","
        + std::to_string(velocityY) + ","
        + std::to_string(velocityZ);
    log->trace("Serialized MovingObject as {}", res);
    return res;
}