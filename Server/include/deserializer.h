#pragma once
#include <string>
#include <map>
#include <memory>
#include "gamestate.h"
#include "ObjectClasses/Factories/objectfactory.h"
#include "EventClasses/Factories/eventfactory.h"
class Deserializer
{
public:
	Deserializer();
	std::shared_ptr<GameState> deserializeGameState(std::string serial);
	std::shared_ptr<Event> deserializeEvent(std::string serial);
private:
	std::map<std::string, std::unique_ptr<IObjectFactory>> gameMapping;
	std::map<std::string, std::unique_ptr<IEventFactory>> eventMapping;
	std::shared_ptr <Object> deserializeObject(std::string serial);
};