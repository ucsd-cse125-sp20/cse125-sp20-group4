#pragma once
#include "stoppingevent.h"

class StopRightEvent : public StoppingEvent {

    public:
    StopRightEvent( const std::string & objectId );
    std::string serialize() const;

};
