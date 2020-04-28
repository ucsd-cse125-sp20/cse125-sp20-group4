#pragma once
#include "movingevent.h"

class MoveForwardEvent : MovingEvent
{
public:
	MoveForwardEvent(int objectId);
	std::string serialize();

};
