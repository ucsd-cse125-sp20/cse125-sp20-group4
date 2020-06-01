#include "EventClasses/Object/Movement/moverightevent.h"

MoveRightEvent::MoveRightEvent( const std::string & objectId ) : MovingEvent( objectId, glm::vec3( -1.0f, 0.0f, 0.0f ) ) {}

std::string MoveRightEvent::serialize() const {

    return "MoveRight:" + Event::serialize();

}