#pragma once
#include "EventClasses/GameState/gamestateevent.h"
class PlaceBarricadeEvent : public GameStateEvent
{
public:
	PlaceBarricadeEvent(std::string id);

	void apply(GameState* gamestate) const;
	std::string serialize() const;
};