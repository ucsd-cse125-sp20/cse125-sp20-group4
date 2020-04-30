#pragma once
#include "movingevent.h"

class MoveBackwardEvent : public MovingEvent
{
public:
	MoveBackwardEvent(std::string objectId);
	std::string serialize();
};
