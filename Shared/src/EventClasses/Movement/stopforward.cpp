#include "EventClasses/Movement/stopforwardevent.h"

StopForwardEvent::StopForwardEvent( const std::string & objectId ) : StoppingEvent( objectId, glm::vec3( 0.0f, 0.0f, 1.0f ) ) {}

std::string StopForwardEvent::serialize() const {

    return "StopForward:" + Event::serialize();

}