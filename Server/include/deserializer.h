#pragma once
#include <string>
#include <map>
#include "ObjectClasses/object.h"
#include "EventClasses/event.h"
class Deserializer
{
public:
	Deserializer();
	GameState& deserializeGameState(std::string serial);
	Event& deserializeEvent(std::string serial);
private:
	std::map<std::string, Object&> gameMapping;
	std::map<std::string, Event&> eventMapping;
};