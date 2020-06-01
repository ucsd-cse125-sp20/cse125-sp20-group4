#pragma once
#include "stoppingevent.h"

class StopForwardEvent : public StoppingEvent {

    public:
    StopForwardEvent( const std::string & objectId );
    std::string serialize() const;

};
