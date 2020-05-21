#pragma once
#include "stoppingevent.h"

class StopBackwardEvent : public StoppingEvent
{
public:
	StopBackwardEvent(std::string objectId);
	std::string serialize();
};
