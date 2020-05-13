#include "gameState.h"
#include "logger.h"


GameState::GameState() {
    this->nextId = 0;
    this->gameObjects = std::map<std::string, std::shared_ptr<Object>>();
    this->dirty = true;
}

void GameState::createObject(std::shared_ptr<Object> obj) {
    auto log = getLogger("GameState");
    //obj->setId(std::to_string(this->nextId));
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
    auto log = getLogger("GameState");
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
        glm::vec3 cpos = it->second->getPosition();
        if (x != cpos.x || y != cpos.y || z != cpos.z) {
            it->second->dirty = true;
            setDirty(true);
            log->trace("Set dirty bit of object with id: {} to true", it->second->getId());
        }

        // set new state
        it->second->setPosition(x, y, z);
        
        it++;
    }
}

void GameState::applyEvent(std::shared_ptr<Event> event) {
    auto log = getLogger("GameState");
    log->info("Applying an Event: {}", event->serialize());
    std::map<std::string, std::shared_ptr<Object>>::iterator it = gameObjects.find(event->getObjectId());
    if (it != gameObjects.end()) {
        event->apply(it->second);
    }
}

std::string GameState::serialize() {
    auto log = getLogger("GameState");
    log->trace("Beginning to serialize GameState");
    auto it = this->gameObjects.begin();

    std::string res = "GameState:";
    while (it != this->gameObjects.end()) {
        res = res + it->second->serialize() + ";";
        it++;
    }
    res = res + "|";
    log->trace("Serialized GameState with state: {}", res);
    return res;
}
void GameState::initialize(std::string file) {
    if (file.compare("") == 0) {
        // default
        // create a player
        std::shared_ptr<Object> obj = std::shared_ptr<Object>(new Player("cube1",0,0,0,1.0f,0.0f,0.0f,0.1f,0.0f,0.0f));
        std::cout << "INITIALIZING DEFAULT" << std::endl;
        this->createObject(obj);
        std::cout << this->serialize() << std::endl;
    } else {
        // TODO parse file
        std::cout << "INITIALIZING FROM A FILE IS NOT IMPLEMENTED" << std::endl;
    }
 }

std::string GameState::getUpdates() {
    // loop over all objects and build string
    auto log = getLogger("GameState");
    log->trace("Beginning to get updates from GameState");
    auto it = this->gameObjects.begin();

    std::string res = "";
    while (it != this->gameObjects.end()) {
        if (it->second->dirty == true) {
            if (res.compare("") == 0) {
                res = it->second->serialize();
            } else {
                res = res + ";" + it->second->serialize();
            }
            it->second->dirty = false;
        }
        it++;
    }
    setDirty(false);
    log->trace("Updates: {}", res);
    return res;
}

bool GameState::isDirty() {
    return this->dirty;
}

void GameState::setDirty(bool dty) {
    this->dirty = dty;
}