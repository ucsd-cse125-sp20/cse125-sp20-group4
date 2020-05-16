#include "deserializer.h"
#include "gamestate.h"
#include "ObjectClasses/object.h"
#include "EventClasses/event.h"
#include "ObjectClasses/Factories/playerfactory.h"
#include "EventClasses/Factories/eventmovementfactories.h"
#include "EventClasses/Factories/eventstopfactories.h"
#include "logger.h"

#include <cstddef>


Deserializer::Deserializer() {
    this->gameMapping.insert(std::make_pair("Player", std::unique_ptr<PlayerFactory>(new PlayerFactory())));
    this->eventMapping.insert(std::make_pair("MoveLeft", std::unique_ptr<MoveLeftEventFactory>(new MoveLeftEventFactory())));
    this->eventMapping.insert(std::make_pair("MoveRight", std::unique_ptr<MoveRightEventFactory>(new MoveRightEventFactory())));
    this->eventMapping.insert(std::make_pair("MoveForward", std::unique_ptr<MoveForwardEventFactory>(new MoveForwardEventFactory())));
    this->eventMapping.insert(std::make_pair("MoveBackward", std::unique_ptr<MoveBackwardEventFactory>(new MoveBackwardEventFactory())));
    this->eventMapping.insert(std::make_pair("StopLeft", std::unique_ptr<StopLeftEventFactory>(new StopLeftEventFactory())));
    this->eventMapping.insert(std::make_pair("StopRight", std::unique_ptr<StopRightEventFactory>(new StopRightEventFactory())));
    this->eventMapping.insert(std::make_pair("StopForward", std::unique_ptr<StopForwardEventFactory>(new StopForwardEventFactory())));
    this->eventMapping.insert(std::make_pair("StopBackward", std::unique_ptr<StopBackwardEventFactory>(new StopBackwardEventFactory())));
}

void Deserializer::deserializeUpdates(std::string serial, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Object>>> res) {
    // get the tag
    auto log = getLogger("Deserialize");
    size_t pos = serial.find(":");
    size_t end = serial.find("|");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    log->trace("Deserializing Updates");
    if (tag.compare("GameState") != 0) {
        //error
        throw "Deserializer: Non gamestate serialization passed in";
    }
    std::string delimiter = ";";
    std::string token;
    // loop over the string while there is still a delimiter
    while ((pos = serial.find(delimiter,last)) != std::string::npos && pos<end) {
        token = serial.substr(last, pos-last);
        last = pos + 1;
        //deserialize the object
        std::shared_ptr<Object> obj = deserializeObject(token);
        std::pair < std::string, std::shared_ptr<Object>> pair(std::string(obj->getId()), obj);
        res->insert(pair);
    }
}
std::shared_ptr<Event> Deserializer::deserializeEvent(std::string serial) {
    std::string tag = serial.substr(0, serial.find(":"));
    std::string input(serial);
    return this->eventMapping.find(tag)->second->create(input);
}
std::shared_ptr <Object> Deserializer::deserializeObject(std::string serial) {
    auto log = getLogger("Deserialize");
    //get the object tag
    std::string tag = serial.substr(0, serial.find(":"));
    log->trace("Deserializing {}", tag);
    return this->gameMapping.find(tag)->second->create(serial);
}