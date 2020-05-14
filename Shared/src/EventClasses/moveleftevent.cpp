#include "EventClasses/moveleftevent.h"

MoveLeftEvent::MoveLeftEvent(std::string objectId) : MovingEvent(objectId, glm::mat3(0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)){}
std::string MoveLeftEvent::serialize() {
    return "MoveLeft:" + Event::serialize();
}