#pragma once
#include "EventClasses/event.h"

Event::Event( const std::string & objectId, const EventType type ) : objectId( objectId ), type (type){}

const std::string & Event::getObjectId() const {

    return this->objectId;

}

std::string Event::serialize() const {

    std::string res = getObjectId();
    return res;

}

const Event::EventType Event::getType() const {

    return this->type;

}