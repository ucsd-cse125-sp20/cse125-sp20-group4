#pragma once
#include "movingevent.h"

class MoveRightEvent : public MovingEvent {

    public:
    MoveRightEvent( const std::string & objectId );
    std::string serialize() const;

};
