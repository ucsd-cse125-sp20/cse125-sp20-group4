#include "deserializer.h"
#include "gamestate.h"
#include "ObjectClasses/object.h"
#include "EventClasses/event.h"
#include "ObjectClasses/Factories/playerfactory.h"
#include "EventClasses/Factories/eventmovementfactories.h"
#include <cstddef>

Deserializer::Deserializer() {
    this->gameMapping.insert(std::make_pair("Player", std::unique_ptr<PlayerFactory>(new PlayerFactory())));
    this->eventMapping.insert(std::make_pair("MoveLeft", std::unique_ptr<MoveLeftEventFactory>(new MoveLeftEventFactory())));
    this->eventMapping.insert(std::make_pair("MoveRight", std::unique_ptr<MoveRightEventFactory>(new MoveRightEventFactory())));
    this->eventMapping.insert(std::make_pair("MoveForward", std::unique_ptr<MoveForwardEventFactory>(new MoveForwardEventFactory())));
    this->eventMapping.insert(std::make_pair("MoveBackward", std::unique_ptr<MoveBackwardEventFactory>(new MoveBackwardEventFactory())));
}

std::shared_ptr<GameState> Deserializer::deserializeGameState(std::string serial) {
    // get the tag
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    
    if (tag.compare("GameState") != 0) {
        //error
        throw "Deserializer: Non gamestate serialization passed in";
    }
    std::shared_ptr<GameState> gs = std::shared_ptr<GameState>(new GameState());
    std::string delimiter = ";";
    std::string token;
    // loop over the string while there is still a delimiter
    while ((pos = serial.find(delimiter,last)) != std::string::npos) {
        token = serial.substr(last, pos-last);
        last = pos + 1;
        //deserialize the object
        std::shared_ptr<Object> obj = deserializeObject(token);
        gs->createObject(obj);
    }
    return gs;
}
std::shared_ptr<Event> Deserializer::deserializeEvent(std::string serial) {
    std::string tag = serial.substr(0, serial.find(":"));
    return this->eventMapping.find(tag)->second->create(serial);
}
std::shared_ptr <Object> Deserializer::deserializeObject(std::string serial) {
    //get the object tag
    std::string tag = serial.substr(0, serial.find(":"));
    return this->gameMapping.find(tag)->second->create(serial);
}