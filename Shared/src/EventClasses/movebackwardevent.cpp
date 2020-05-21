#include "EventClasses/movebackwardevent.h"

MoveBackwardEvent::MoveBackwardEvent( const std::string & objectId ) : MovingEvent( objectId, glm::vec3( 0.0f, 0.0f, -1.0f ) ) {}

std::string MoveBackwardEvent::serialize() const {

    return "MoveBackward:" + Event::serialize();

}