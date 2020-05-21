#pragma once
#include "stoppingevent.h"

class StopBackwardEvent : public StoppingEvent {

    public:
    StopBackwardEvent( const std::string & objectId );
    std::string serialize() const;

};
