#pragma once
#include "EventClasses/event.h"

Event::Event(std::string objectId) {
    this->objectId = objectId;
}
std::string Event::getObjectId() {
    return this->objectId;
}

std::string Event::serialize() {
    std::string res = getObjectId();
    return res;
}