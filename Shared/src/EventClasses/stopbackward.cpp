#include "EventClasses/stopbackwardevent.h"

StopBackwardEvent::StopBackwardEvent( const std::string & objectId ) : StoppingEvent( objectId, glm::vec3( 0.0f, 0.0f, -1.0f ) ) {}

std::string StopBackwardEvent::serialize() const {

    return "StopBackward:" + Event::serialize();

}