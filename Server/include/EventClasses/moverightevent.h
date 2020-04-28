#pragma once
#include "movingevent.h"

class MoveRightEvent : MovingEvent
{
public:
	MoveRightEvent(int objectId);
	std::string serialize();
};
