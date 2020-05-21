#pragma once
#include "movingevent.h"

class MoveLeftEvent : public MovingEvent {

    public:
    MoveLeftEvent( const std::string & objectId );
    std::string serialize() const;

};
