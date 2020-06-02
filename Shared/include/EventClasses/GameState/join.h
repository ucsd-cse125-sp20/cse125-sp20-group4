#pragma once
#include "EventClasses/GameState/gamestateevent.h"
class JoinEvent : public GameStateEvent
{
public:
	JoinEvent(std::string id);

	void apply(GameState* gamestate) const;
	std::string serialize() const;
	static const std::string TAG;
};