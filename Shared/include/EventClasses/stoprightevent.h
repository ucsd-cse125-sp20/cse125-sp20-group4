#pragma once
#include "stoppingevent.h"

class StopRightEvent : public StoppingEvent
{
public:
	StopRightEvent(std::string objectId);
	std::string serialize();
};
