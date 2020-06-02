#pragma once
#include "EventClasses/GameState/gamestateevent.h"
class PlaceEvent : public GameStateEvent
{
public:
	PlaceEvent(std::string id);

	void apply(GameState* gamestate) const;
	std::string serialize() const;
};