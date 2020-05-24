#pragma once
#include "EventClasses/event.h"
#include "gameState.h"
class ServerEvent : public Event
{
public:
	ServerEvent(const std::string& objectId);
	virtual void apply(std::shared_ptr<GameState> object) const = 0;

};