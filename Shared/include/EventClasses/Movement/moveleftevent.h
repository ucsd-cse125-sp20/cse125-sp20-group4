#pragma once
#include "movingevent.h"

class MoveLeftEvent : public MovingEvent
{
public:
	MoveLeftEvent(std::string objectId);
	std::string serialize();
};
