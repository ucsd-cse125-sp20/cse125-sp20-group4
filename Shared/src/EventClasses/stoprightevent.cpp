#include "EventClasses/stoprightevent.h"

StopRightEvent::StopRightEvent(std::string objectId) : StoppingEvent(objectId, glm::mat3(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f)) {}
std::string StopRightEvent::serialize() {
    return "StopRight:" + Event::serialize();
}