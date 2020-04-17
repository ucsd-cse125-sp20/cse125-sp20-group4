#include "gameState.h"
GameState::GameState() {
    this->nextId = 0;
    this->gameObjects = std::map<int, Object&>();
}
void GameState::updateObject(int id) {
    //TODO
}
void GameState::createObject(Object& obj) {
    obj.setId(this->nextId);
    this->gameObjects.insert(std::pair<int, Object&>(nextId, obj));
    this->nextId++;
}
void GameState::deleteObject(int id) {
    std::map<int, Object&>::iterator it;
    it = this->gameObjects.find(id);
    if (it != this->gameObjects.end()) {
        this->gameObjects.erase(it);
    }
    //TODO add logging
}
std::string GameState::serialize() {
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
    return res;
}