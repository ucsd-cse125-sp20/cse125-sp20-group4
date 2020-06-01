#pragma once
#include "EventClasses/GameState/gamestateevent.h"
class ReadyEvent : public GameStateEvent
{
public:
	ReadyEvent(std::string id);

	void apply(GameState* gamestate) const;
	std::string serialize() const;
	static const std::string TAG;
};