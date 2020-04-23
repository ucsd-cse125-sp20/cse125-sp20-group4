#pragma once
#include "EventClasses/event.h"

Event::Event(int objectId) {
    this->objectId = objectId;
}
int Event::getObjectId() {
    return this->objectId;
}

std::string Event::serialize() {
    std::string res = std::to_string(getObjectId());
    return res;
}