#include "EventClasses/stopbackwardevent.h"

StopBackwardEvent::StopBackwardEvent(std::string objectId) : StoppingEvent(objectId, glm::mat3(-1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f)) {}
std::string StopBackwardEvent::serialize() {
    return "StopBackward:" + Event::serialize();
}