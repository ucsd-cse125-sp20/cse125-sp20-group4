#pragma once
#include "event.h"
class MoveLeftEvent : Event
{
public:
	MoveLeftEvent(int objectId);
	void apply(Object& object);
	virtual float getMovementDirection();
};
