#include "EventClasses/Movement/moveleftevent.h"

MoveLeftEvent::MoveLeftEvent(const std::string & objectId) : MovingEvent(objectId, glm::vec3(-1.0f, 0.0f, 0.0f)) {}
std::string MoveLeftEvent::serialize() const {
    return "MoveLeft:" + Event::serialize();
}