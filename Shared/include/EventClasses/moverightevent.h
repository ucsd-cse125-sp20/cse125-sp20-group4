#pragma once
#include "movingevent.h"

class MoveRightEvent : public MovingEvent
{
public:
	MoveRightEvent(std::string objectId);
	std::string serialize();
};
