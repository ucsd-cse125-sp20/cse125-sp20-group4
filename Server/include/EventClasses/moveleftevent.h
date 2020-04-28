#pragma once
#include "movingevent.h"

class MoveLeftEvent : MovingEvent
{
public:
	MoveLeftEvent(int objectId);
	std::string serialize();
};
