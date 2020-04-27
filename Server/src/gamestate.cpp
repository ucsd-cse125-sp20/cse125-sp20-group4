#include "gameState.h"
#include "logger.h"

GameState::GameState() {
    this->nextId = 0;
    this->gameObjects = std::map<int, Object&>();
}

void GameState::createObject(Object& obj) {
    auto log = getLogger("GameState");
    obj.setId(std::to_string(this->nextId));
    this->gameObjects.insert(std::pair<int, Object&>(nextId, obj));
    this->nextId++;
    log->trace("Created GameState object with id: {}", this->nextId);
}

void GameState::deleteObject(int id) {
    auto log = getLogger("GameState");
    log->trace("Deleting GameState object with id: {}", id);
    std::map<int, Object&>::iterator it;
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
    std::map<int, Object&>::iterator it = this->gameObjects.begin();
    float x;
    float y;
    float z;
    while (it != this->gameObjects.end()) {
        // calculate next position
        x = it->second.getNextPositionX();
        y = it->second.getNextPositionY();
        z = it->second.getNextPositionZ();
        // TODO check for collisions
        // set new state
        it->second.setPosition(x, y, z);
    }
}

void GameState::applyEvent(Event* event) {
    std::map<int, Object&>::iterator it = gameObjects.find(event->getObjectId());
    if (it != gameObjects.end()) {
        event->apply(it->second);
    }
}

std::string GameState::serialize() {
    auto log = getLogger("GameState");
    log->trace("Beginning to serialize GameState");
    std::map<int, Object&>::iterator it = this->gameObjects.begin();
    std::string res = "";
    while (it != this->gameObjects.end()) {
        if (res.compare("") == 0) {
            res = it->second.serialize();
        }
        else {
            res = res + ";" + it->second.serialize();
        }
        it++;
    }
    log->trace("Serialized GameState with state: {}", res);
    return res;
}