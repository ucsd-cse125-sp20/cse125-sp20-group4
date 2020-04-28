#include "EventClasses/moveleftevent.h"

MoveLeftEvent::MoveLeftEvent(int objectId) : MovingEvent(objectId, glm::mat3(0.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)){}