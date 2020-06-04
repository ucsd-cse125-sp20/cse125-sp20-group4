#pragma once
#include "EventClasses/event.h"
#include "phases/phase.h"
class UpdatePhaseEvent : public Event
{
public:

	Phase phase;
	UpdatePhaseEvent(std::shared_ptr<Phase>);
	std::string serialize() const;
};