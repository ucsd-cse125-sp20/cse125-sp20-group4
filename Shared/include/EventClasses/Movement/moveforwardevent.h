#pragma once
#include "movingevent.h"

class MoveForwardEvent : public MovingEvent
{
public:
	MoveForwardEvent(std::string objectId);
	std::string serialize();

};
