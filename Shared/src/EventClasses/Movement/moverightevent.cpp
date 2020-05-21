#include "EventClasses/Movement/moverightevent.h"

MoveRightEvent::MoveRightEvent(std::string objectId) : MovingEvent(objectId, glm::mat3(0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f)) {}
std::string MoveRightEvent::serialize() {
    return "MoveRight:" + Event::serialize();
}