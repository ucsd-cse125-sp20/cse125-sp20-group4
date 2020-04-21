#include "gameState.h"
#include "logger.h"
GameState::GameState() {
    this->nextId = 0;
    this->gameObjects = std::map<int, Object&>();
}
void GameState::updateObject(int id) {
    //TODO
}
void GameState::createObject(Object& obj) {
    auto log = logger();
    obj.setId(this->nextId);
    this->gameObjects.insert(std::pair<int, Object&>(nextId, obj));
    this->nextId++;
    log->trace("Created GameState object with id: {}", this->nextId);
}
void GameState::deleteObject(int id) {
    auto log = logger();
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
    //TODO add logging
}
std::string GameState::serialize() {
    auto log = logger();
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