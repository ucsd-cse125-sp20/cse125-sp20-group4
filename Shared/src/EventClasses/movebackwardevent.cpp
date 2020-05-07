#include "EventClasses/movebackwardevent.h"

MoveBackwardEvent::MoveBackwardEvent(std::string objectId) : MovingEvent(objectId, glm::mat3(-1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f)) {}
std::string MoveBackwardEvent::serialize() {
    return "MoveBackward:" + Event::serialize();
}