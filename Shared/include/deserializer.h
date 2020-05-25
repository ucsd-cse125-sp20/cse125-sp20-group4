#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include <memory>
#include "gamestate.h"
#include "ObjectClasses/Factories/objectfactory.h"
#include "EventClasses/Factories/eventfactory.h"
class Deserializer
{
public:
	Deserializer();
	std::string deserializeUpdates(std::string serial, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Object>>> res);
	std::shared_ptr<Event> deserializeEvent( std::string serial );
	std::shared_ptr <Object> deserializeObject( std::string serial );
private:
	std::map<std::string, std::unique_ptr<IObjectFactory>> gameMapping;
	std::map<std::string, std::unique_ptr<IEventFactory>> eventMapping;
	std::unordered_map<std::string, std::shared_ptr<Object>> updates;
};