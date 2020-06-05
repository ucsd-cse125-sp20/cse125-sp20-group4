#pragma once
#include "EventClasses/GameState/gamestateevent.h"
class PlaceEvent : public GameStateEvent
{
public:
	glm::vec3 pos;
	PlaceEvent(std::string id, glm::vec3& pos);

	void apply(GameState* gamestate) const;
	std::string serialize() const;
};