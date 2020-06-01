#pragma once
#include "gameState.h"
#include "EventClasses/event.h"
class GameStateEvent : public Event
{
public:
	GameStateEvent(std::string id);
	virtual void apply(GameState* gamestate) const = 0;
};