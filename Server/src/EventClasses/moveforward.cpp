#include "EventClasses/moveforwardevent.h"

MoveRightEvent::MoveRightEvent(int objectId) : MovingEvent(objectId, glm::mat3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f)) {}