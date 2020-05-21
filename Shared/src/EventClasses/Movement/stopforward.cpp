#include "EventClasses/Movement/stopforwardevent.h"

StopForwardEvent::StopForwardEvent(std::string objectId) : StoppingEvent(objectId, glm::mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)) {}

std::string StopForwardEvent::serialize() {
    return "StopForward:"+Event::serialize();
}