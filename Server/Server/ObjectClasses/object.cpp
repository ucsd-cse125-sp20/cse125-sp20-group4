#include "object.h"

Object::Object(int id) {
    this->id = id;
    setPosition(0, 0, 0);
    setOrientation(0.0f, 0.0f);
}
Object::Object(int id, int x, int y, int z) {
    this->id = id;
    setPosition(x, y, z);
    setOrientation(0.0f, 0.0f);
}

Object::Object(int id, int x, int y, int z, float orientationX, float orientationY) {
    this->id = id;
    setPosition(x, y, z);
    setOrientation(orientationX, orientationY);
}

void Object::setId(int id) {
    this->id = id;
}

void Object::setPositionX(int x) {
    this->x = x;
}

void Object::setPositionY(int y) {
    this->y = y;
}

void Object::setPositionZ(int z) {
    this->z = z;
}

void Object::setPosition(int x, int y, int z) {
    setPositionX(x);
    setPositionY(y);
    setPositionZ(z);
}

void Object::setOrientationY(float orientationY) {
    this->orientationY = orientationY;
}

void Object::setOrientationX(float orientationX) {
    this->orientationX = orientationX;
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
    // id, x, y, z, directionH, directionV
    std::string res = std::to_string(getId()) + ","
        + std::to_string(getPositionX()) + ","
        + std::to_string(getPositionY()) + ","
        + std::to_string(getPositionZ()) + ","
        + std::to_string(getOrientationY()) + ","
        + std::to_string(getOrientationX());
    return res;
}