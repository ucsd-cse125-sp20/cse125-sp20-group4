#pragma once
#include "movingevent.h"

class MoveBackwardEvent : MovingEvent
{
public:
	MoveBackwardEvent(int objectId);
	std::string serialize();

};
