#pragma once
#include "EventClasses/event.h"

Event::Event( const std::string & objectId ) : objectId( objectId ) {}

const std::string & Event::getObjectId() const {

    return this->objectId;

}

std::string Event::serialize() const {

    std::string res = getObjectId();
    return res;

}