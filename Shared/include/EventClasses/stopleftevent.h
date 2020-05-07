#pragma once
#include "stoppingevent.h"

class StopLeftEvent : public StoppingEvent
{
public:
	StopLeftEvent(std::string objectId);
	std::string serialize();
};
