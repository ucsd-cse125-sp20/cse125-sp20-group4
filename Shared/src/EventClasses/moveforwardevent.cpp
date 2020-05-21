#include "EventClasses/moveforwardevent.h"

MoveForwardEvent::MoveForwardEvent( const std::string & objectId ) : MovingEvent( objectId, glm::vec3( 0.0f, 0.0f, 1.0f ) ) {}

std::string MoveForwardEvent::serialize() const {

    return "MoveForward:" + Event::serialize();

}