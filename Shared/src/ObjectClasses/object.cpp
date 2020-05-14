#include "ObjectClasses/object.h"
#include "logger.h"

Object::Object(const Object& obj) : Object(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(), obj.getOrientationX(), obj.getOrientationY(), obj.getOrientationZ()) {}

Object::Object(std::string id) : Object(id, 0.0f, 0.0f, 0.0f) {}

Object::Object(std::string id, float x, float y, float z) : Object(id, x, y, z, 0.0f, 0.0f, 0.0f) {}

Object::Object(std::string id, float x, float  y, float z, float dirX, float dirY, float dirZ) {
    auto log = getLogger("Object");
    this->id = id;
    setPosition(x, y, z);
    setOrientation(dirX, dirY, dirZ);
    log->trace("Creating Object with id {}, position ({}, {}, {}), and orientation ({}, {}, {})", id, x, y, z, dirX, dirY, dirZ);
}

void Object::setId(std::string inputId) {
    auto log = getLogger("Object");
    this->id = inputId;
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
    auto log = getLogger("Object");
    this->position = glm::vec3(x, y, z);
    log->trace("Setting position of Object {} to ({},{},{})", this->getId(), x, y, z);

}

void Object::setOrientationY(float orientationY) {
    this->setOrientation(this->getOrientation().x, orientationY,  this->getOrientation().z);

}

void Object::setOrientationX(float orientationX) {
    this->setOrientation(orientationX, this->getOrientation().y, this->getOrientation().z);
}

void Object::setOrientationZ(float orientationZ) {
    this->setOrientation(this->getOrientation().x, this->getOrientation().y, orientationZ);
}

void Object::setOrientation(float orientationX, float orientationY, float orientationZ) {
    auto log = getLogger("Object");
    this->orientation = glm::vec3(orientationX, orientationY, orientationZ);
    log->trace("Setting orientation of Object {} to ({},{},{})", this->getId(), orientationX, orientationY, orientationZ);

}

std::string Object::getId() const {
    return this->id;
}

float Object::getPositionX() const {
    return this->getPosition().x;
}

float Object::getPositionY() const {
    return this->getPosition().y;
}

 float Object::getPositionZ() const {
    return this->getPosition().z;
}

glm::vec3 Object::getPosition() const{
    return this->position;
}

float Object::getOrientationX() const {
    return this->orientation.x;
}

float Object::getOrientationY() const {
    return this->orientation.y;
}

float Object::getOrientationZ() const {
    return this->orientation.z;
}

glm::vec3 Object::getOrientation() const {
    return this->orientation;
}

float Object::getNextPositionX() const {
    return getPositionX();
}

float Object::getNextPositionY() const {
    return getPositionY();
}

float Object::getNextPositionZ() const {
    return getPositionZ();
}

float Object::getLength() const {
    return this->length;
}

float Object::getWidth() const {
    return this->width;
}

float Object::getHeight() const {
    return this->height;
}

std::string Object::serialize() {
    auto log = getLogger("Object");
    // id, x, y, z, directionX, directionY, directionZ
    std::string res = getId() + ","
        + std::to_string(getPositionX()) + ","
        + std::to_string(getPositionY()) + ","
        + std::to_string(getPositionZ()) + ","
        + std::to_string(getOrientationX()) + ","
        + std::to_string(getOrientationY()) + ","
        + std::to_string(getOrientationZ());
    log->trace("Serialized Object as {}", res);
    return res;
}