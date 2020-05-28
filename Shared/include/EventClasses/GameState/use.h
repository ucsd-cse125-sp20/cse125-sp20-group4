#pragma once
#include "EventClasses/GameState/gamestateevent.h"
class UseEvent : public GameStateEvent
{
public:
	UseEvent(std::string id);

	void apply(GameState* gamestate) const;
	std::string serialize() const;
};