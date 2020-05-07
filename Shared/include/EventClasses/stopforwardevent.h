#pragma once
#include "stoppingevent.h"

class StopForwardEvent : public StoppingEvent
{
public:
	StopForwardEvent(std::string objectId);
	std::string serialize();

};
