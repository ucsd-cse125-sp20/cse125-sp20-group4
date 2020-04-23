#pragma once
#include "event.h"
class MoveLeftEvent : Event
{
public:
	MoveLeftEvent();
	void apply(Object& object);
	virtual float getMovementDirection();
};
