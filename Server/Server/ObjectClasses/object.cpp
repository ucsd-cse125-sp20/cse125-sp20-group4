#include "object.h"
#include "Shared/logger.h"


Object::Object(int id) : Object(id, 0f, 0f, 0f) {}

Object::Object(int id, float x, float y, float z) : Object(id, x, y, z, 0f, 0f, 0f) {}

Object::Object(int id, float x, float  y, float z, float dirX, float dirY, float dirZ) {
    auto log = logger();
    this->id = id;
    setPosition(x, y, z);
    setOrientation(dirX, dirY, dirZ);
    log->trace("Creating Object with id {}, position ({}, {}, {}), and orientation ({}, {}, {})", id, x, y, z, dirX, dirY, dirZ);
}

void Object::setId(int id) {
    auto log = logger();
    this->id = id;
    log->trace("Setting id of Object to {}", id);
}

void Object::setPositionX(float x) {
    this->setPosition(x, this->getPosition().y, this->getPosition().z);
}

void Object::setPositionY(float y) {
    this->setPosition(this->getPosition().x, y, this->getPosition().z);
}

void Object::setPositionZ(float z) {
    this->setPosition(this->getPosition().x, this->getPosition().y, z);
}

void Object::setPosition(float x, float y, float z) {
    auto logger = logger();
    this->location = glm::vec3(x, y, z);
    log->trace("Setting position of Object {} to {}", this->getId(), this->location);

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

void Object::setOrientationZ(float orientationZ) {
    this->setOrientation()
}

void Object::setOrientation(float orientationX, float orientationY, float orientationZ) {
    auto log = logger();
    this->orientation = glm::vec3(orientationX, orientationY, orientationZ);
    log->trace("Setting orientation of Object {} to {}", this->getId(), orientationX);

}

int Object::getId() {
    return this->id;
}

float Object::getPositionX() {
    return this->x;
}

float Object::getPositionY() {
    return this->y;
}

float Object::getPositionZ() {
    return this->z;
}

float Object::getOrientationX() {
    return this->orientation.x;
}

float Object::getOrientationY() {
    return this->orientation.y;
}

float Object::getOrientationZ() {
    return this->orientation.z;
}

glm::vec3 Object::getOrientation() {
    return this->orientation;
}

std::string Object::serialize() {
    auto log = logger();
    // id, x, y, z, directionH, directionV
    std::string res = std::to_string(getId()) + ","
        + std::to_string(getPositionX()) + ","
        + std::to_string(getPositionY()) + ","
        + std::to_string(getPositionZ()) + ","
        + std::to_string(getOrientationX()) + ","
        + std::to_string(getOrientationY()) + ","
        + std::to_string(getOrientationZ());
    log->trace("Serialized Object as {}", res);
    return res;
}