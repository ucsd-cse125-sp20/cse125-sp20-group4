#include "object.h"

Object::Object(int id) {
    id = id;
    x = 0;
    y = 0;
    z = 0;
    directionHorizontal = 0.0f;
    directionVertical = 0.0f;
}
Object::Object(int id, int x, int y, int z) {
    id = id;
    setPosition(x, y, z);
    setDirection(0.0f, 0.0f);
}

Object::Object(int id, int x, int y, int z, float hor, float ver) {
    id = id;
    setPosition(x, y, z);
    setDirection(hor, ver);
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

void Object::setDirectionHorizontal(float hor) {
    this->directionHorizontal = hor;
}

void Object::setDirectionVertical(float ver) {
    this->directionVertical = ver;
}

void Object::setDirection(float hor, float ver) {
    setDirectionHorizontal(hor);
    setDirectionVertical(ver);
}

int Object::getId() {
    return this->id;
}

int Object::getPositionX() {
    return this->x;
}

int Object::getPositionY() {
    return this->Y;
}

int Object::getPositionZ() {
    return this->Z;
}

float Object::getDirectionHorizontal() {
    return this->directionHorizontal;
}

float Object::getDirectionVertical() {
    return this->directionVertical;
}