#pragma once
#include "movingevent.h"

class MoveForwardEvent : public MovingEvent {

    public:
    MoveForwardEvent( const std::string & objectId );
    std::string serialize() const;

};
