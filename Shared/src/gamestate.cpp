#include "gameState.h"
#include "logger.h"


GameState::GameState() {
    this->nextId = 0;
    this->gameObjects = std::map<std::string, std::shared_ptr<Object>>();
}

void GameState::createObject(std::shared_ptr<Object> obj) {
    auto log = getLogger("GameState");
    obj->setId(std::to_string(this->nextId));
    this->gameObjects.insert(std::make_pair(obj->getId(), obj));
    this->nextId++;
    log->trace("Created GameState object with id: {}", this->nextId);
}

void GameState::createObject(std::shared_ptr<Object> obj, std::string id) {
    auto log = getLogger("GameState");
    this->gameObjects.insert(std::pair<std::string, std::shared_ptr<Object>>(id, obj));
    log->trace("Created GameState object with id: {}", this->nextId);
}

void GameState::deleteObject(std::string id) {
    auto log = getLogger("GameState");
    log->trace("Deleting GameState object with id: {}", id);
    std::map<std::string, std::shared_ptr<Object>>::iterator it;
    it = this->gameObjects.find(id);
    if (it != this->gameObjects.end()) {
        this->gameObjects.erase(it);
        log->trace("Deleted GameState object with id: {}", id);
    }
    else {
        log->trace("Attempted to delete object with id: {} but did not find", id);
    }
}

void GameState::updateState() {
    //loop through all objects
    std::map<std::string, std::shared_ptr<Object>>::iterator it = this->gameObjects.begin();
    float x;
    float y;
    float z;
    while (it != this->gameObjects.end()) {
        // calculate next position
        x = it->second->getNextPositionX();
        y = it->second->getNextPositionY();
        z = it->second->getNextPositionZ();
        // TODO check for collisions
        // set new state
        it->second->setPosition(x, y, z);
    }
}

void GameState::applyEvent(std::shared_ptr<Event> event) {
    std::map<std::string, std::shared_ptr<Object>>::iterator it = gameObjects.find(event->getObjectId());
    if (it != gameObjects.end()) {
        event->apply(it->second);
    }
}

std::string GameState::serialize() {
    auto log = getLogger("GameState");
    log->trace("Beginning to serialize GameState");
    //std::map<std::string, shared_ptr<Object>>::iterator it = this->gameObjects.begin();
    auto it = this->gameObjects.begin();

    std::string res = "GameState:";
    while (it != this->gameObjects.end()) {
        if (res.compare("") == 0) {
            res = it->second->serialize();
        }
        else {
            res = res + ";" + it->second->serialize();
        }
        it++;
    }
    log->trace("Serialized GameState with state: {}", res);
    return res;
}
void GameState::initialize(std::string file) {
    if (file.compare("") == 0) {
        // default
        // create a player
        std::shared_ptr<Player> obj = std::make_shared<Player>("1"); //TODO this causes xmemory to complain about copy ctor
        //this->createObject();
    } else {
        // TODO parse file
        std::cout << "INITIALIZING FROM A FILE IS NOT IMPLEMENTED" << std::endl;
    }
 }
