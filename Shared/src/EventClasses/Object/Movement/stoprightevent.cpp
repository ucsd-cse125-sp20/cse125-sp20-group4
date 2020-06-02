#include "EventClasses/Object/Movement/stoprightevent.h"

StopRightEvent::StopRightEvent( const std::string & objectId ) : StoppingEvent( objectId, glm::vec3( -1.0f, 0.0f, 0.0f ) ) {}

std::string StopRightEvent::serialize() const {

    return "StopRight:" + Event::serialize();

}