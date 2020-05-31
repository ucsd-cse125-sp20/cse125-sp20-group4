#include "ObjectClasses/object.h"
#include "logger.h"
Object::Object(const Object& obj) : Object(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(), obj.getOrientationX(), obj.getOrientationY(), obj.getOrientationZ(), obj.getWidth(), obj.getHeight(), obj.getLength(), obj.canCollide()) {}

Object::Object(std::string id) : Object(id, 0.0f, 0.0f, 0.0f) {}

Object::Object(std::string id, float x, float y, float z) : Object(id, x, y, z, 0.0f, 1.0f, 0.0f) {} //was 1.0f, 0.0f, 0.0f

Object::Object(std::string id, float x, float  y, float z, float dirX, float dirY, float dirZ): Object::Object(id, x, y, z, dirX, dirY, dirZ, 0.0f, 0.0f, 0.0f, false){}

Object::Object(std::string id, float x, float  y, float z, float dirX, float dirY, float dirZ, float width, float height, float length, bool isCollidable) {
    auto log = getLogger("Object");
    this->id = id;
    setPosition(x, y, z);
    setOrientation(dirX, dirY, dirZ);
    dirty = true;
    setWidth(width);
    setHeight(height);
    setLength(length);
    setCanCollide(isCollidable);
    log->trace("Creating Object with id {}, position ({}, {}, {}), width {}, length {}, height {}, and orientation ({}, {}, {})", id, x, y, z, width, length, height, dirX, dirY, dirZ);
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
    this->setOrientation( glm::vec3( orientationX, orientationY, orientationZ ) );
}

void Object::setOrientation( const glm::vec3 & ori ) {

    auto log = getLogger("Object");
    log->trace( "Setting orientation of Object {} to ({},{},{})", this->getId(), ori.x, ori.y, ori.z );
    this->orientation = ori;

}

void Object::setWidth(float newWidth) {
    this->width = newWidth;
}

void Object::setHeight(float newHeight) {
    this->height = newHeight;
}

void Object::setLength(float newLength) {
    this->length = newLength;
}

void Object::setCanCollide(bool newvalue) {
    this->collide = newvalue;
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

float Object::getLength() const { //Z size
    return this->length;
}

float Object::getWidth() const { //X size
    return this->width;
}

float Object::getHeight() const { //Y size
    return this->height;
}

bool Object::canCollide() const {
    return this->collide;
}

bool Object::contains(const glm::vec3 pt) const {
    if (pt.x < this->getPositionX() + (this->getWidth() / 2) && pt.x > this->getPositionX() - (this->getWidth() / 2)) {
        return true;
    }
    if (pt.x < this->getPositionY() + (this->getHeight() / 2) && pt.x > this->getPositionY() - (this->getHeight() / 2)) {
        return true;
    }
    if (pt.x < this->getPositionZ() + (this->getLength() / 2) && pt.x > this->getPositionZ() - (this->getLength() / 2)) {
        return true;
    }
    return false;
    
}

bool Object::collides(const Object & obj) const {
    if (!obj.canCollide()) {
        return false;
    }
    if (obj.getPositionX() - (obj.getWidth() / 2)  < this->getPositionX() + (this->getWidth() / 2) && obj.getPositionX() + (obj.getWidth() / 2) > this->getPositionX() - (this->getWidth() / 2)) {
        return true;
    }
    if (obj.getPositionY() - (obj.getHeight() / 2) < this->getPositionY() + (this->getHeight() / 2) && obj.getPositionY() + (obj.getHeight() / 2) > this->getPositionY() - (this->getHeight() / 2)) {
        return true;
    }
    if (obj.getPositionZ() - (obj.getLength() / 2) < this->getPositionZ() + (this->getLength() / 2) && obj.getPositionZ() + (obj.getLength() / 2) > this->getPositionZ() - (this->getLength() / 2)) {
        return true;
    }
    return false;

}

std::shared_ptr<Object> Object::clone() const {
    return std::make_shared<Object>(*this);
}

std::string Object::serialize() const {
    auto log = getLogger("Object");
    // id, x, y, z, directionX, directionY, directionZ
    std::string res = getId() + ","
        + std::to_string(getPositionX()) + ","
        + std::to_string(getPositionY()) + ","
        + std::to_string(getPositionZ()) + ","
        + std::to_string(getOrientationX()) + ","
        + std::to_string(getOrientationY()) + ","
        + std::to_string(getOrientationZ()) + ","
        + std::to_string(getWidth()) + ","
        + std::to_string(getHeight()) + ","
        + std::to_string(getLength());
    log->trace("Serialized Object as {}", res);
    return res;
}