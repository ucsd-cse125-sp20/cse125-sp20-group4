#include "EventClasses/Movement/stopleftevent.h"

StopLeftEvent::StopLeftEvent( const std::string & objectId ) : StoppingEvent( objectId, glm::vec3( -1.0f, 0.0f, 0.0f ) ) {}

std::string StopLeftEvent::serialize() const {

    return "StopLeft:" + Event::serialize();

}