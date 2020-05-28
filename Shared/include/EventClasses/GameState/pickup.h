#pragma once
#include "EventClasses/GameState/gamestateevent.h"
class PickUpEvent : public GameStateEvent
{
public:
	std::string targetId;
	PickUpEvent(std::string id, std::string targetId);

	void apply(GameState* gamestate) const;
	std::string serialize() const;
};