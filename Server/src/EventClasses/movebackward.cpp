#include "EventClasses/movebackwardevent.h"

MoveBackwardEvent::MoveBackwardEvent(int objectId) : MovingEvent(objectId, glm::mat3(-1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f)) {}