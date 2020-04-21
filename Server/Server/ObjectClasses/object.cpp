#include "object.h"
#include "Shared/logger.h"

Object::Object(int id) : Object(id, 0, 0, 0) {}

Object::Object(int id, int x, int y, int z) : Object(id, x, y, z, 0f, 0f) {}

Object::Object(int id, int x, int y, int z, float orientationX, float orientationY) {
    auto log = logger();
    this->id = id;
    setPosition(x, y, z);
    setOrientation(orientationX, orientationY);
    log->trace("Creating Object with id {}, position ({}, {}, {}), and orientation ({}, {})", id, x, y, z, orientationX, orientationY);
}

void Object::setId(int id) {
    auto log = logger();
    this->id = id;
    log->trace("Setting id of Object to {}", id);
}

void Object::setPositionX(int x) {
    auto log = logger();
    this->x = x;
    log->trace("Setting X position of Object {} to {}", this->getId(), x);
}

void Object::setPositionY(int y) {
    auto log = logger();
    this->y = y;
    log->trace("Setting Y position of Object {} to {}", this->getId(), y);
}

void Object::setPositionZ(int z) {
    auto log = logger();
    this->z = z;
    log->trace("Setting Z position of Object {} to {}", this->getId(), z);
}

void Object::setPosition(int x, int y, int z) {
    setPositionX(x);
    setPositionY(y);
    setPositionZ(z);
}

void Object::setOrientationY(float orientationY) {
    auto log = logger();
    this->orientationY = orientationY;
    log->trace("Setting Y orientation of Object {} to {}", this->getId(), orientationY);
}

void Object::setOrientationX(float orientationX) {
    auto log = logger();
    this->orientationX = orientationX;
    log->trace("Setting X orientation of Object {} to {}", this->getId(), orientationX);
}

void Object::setOrientation(float orientationX, float orientationY) {
    setOrientationX(orientationX);
    setOrientationY(orientationY);
}

int Object::getId() {
    return this->id;
}

int Object::getPositionX() {
    return this->x;
}

int Object::getPositionY() {
    return this->y;
}

int Object::getPositionZ() {
    return this->z;
}

float Object::getOrientationX() {
    return this->orientationX;
}

float Object::getOrientationY() {
    return this->orientationY;
}

std::string Object::serialize() {
    auto log = logger();
    // id, x, y, z, directionH, directionV
    std::string res = std::to_string(getId()) + ","
        + std::to_string(getPositionX()) + ","
        + std::to_string(getPositionY()) + ","
        + std::to_string(getPositionZ()) + ","
        + std::to_string(getOrientationY()) + ","
        + std::to_string(getOrientationX());
    log->trace("Serialized Object as {}", res);
    return res;
}