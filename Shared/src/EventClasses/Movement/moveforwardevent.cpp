#include "EventClasses/Movement/moveforwardevent.h"

MoveForwardEvent::MoveForwardEvent(std::string objectId) : MovingEvent(objectId, glm::mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)) {}

std::string MoveForwardEvent::serialize() {
    return "MoveForward:"+Event::serialize();
}