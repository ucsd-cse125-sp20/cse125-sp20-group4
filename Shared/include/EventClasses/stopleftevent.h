#pragma once
#include "stoppingevent.h"

class StopLeftEvent : public StoppingEvent {

    public:
    StopLeftEvent( const std::string & objectId );
    std::string serialize() const;

};
