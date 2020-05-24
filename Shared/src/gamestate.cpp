#include <ctime>

#include "gameState.h"
#include "logger.h"
#include <EventClasses\Object\ObjectEvent.h>
#include <EventClasses\GameState\gamestateevent.h>

GameState::GameState() {
    this->nextId = 0;
    this->gameObjects = std::map<std::string, std::shared_ptr<Object>>();
    this->timers = std::map<std::string, std::shared_ptr<Timer>>();
    this->dirty = true;
}

void GameState::createObject(std::shared_ptr<Object> obj) {
    createObject(obj, std::to_string(this->nextId++));
}

void GameState::createObject(std::shared_ptr<Object> obj, std::string id) {
    auto log = getLogger("GameState");
    obj->setId(id);
    this->gameObjects.insert(std::pair<std::string, std::shared_ptr<Object>>(id, obj));
    log->trace("Created GameState object with id: {}", id);
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

void GameState::createTimer(std::string id, double duration, std::function<void()> callback) {
    time_t now = time(NULL);
    timers[id] = std::shared_ptr<Timer>(new Timer(now, duration, callback));
}

void GameState::deleteTimer(std::string id) {
    timers.erase(id);
}

std::map<std::string, std::function<void()>> GameState::updateTimers() {
    auto log = getLogger("GameState");
    std::map<std::string, std::function<void()>> callbacks = std::map<std::string, std::function<void()>>();
    time_t now = time(NULL);

    // iterate over timers
    for (auto it = timers.begin(), itNext = it; it != timers.end(); it = itNext) {
        // increment before check/delete so that active iterator not deleted
        itNext++;
        std::function<void()> callback = it->second->update(now);
        // if callback is not null, timer expired
        if (callback) {
            callbacks[it->first] = callback;
            timers.erase(it);
        }
    }
    log->info("Finished updating timers");
    log->trace("{} timers expired of {}", callbacks.size(), timers.size());
    return callbacks;
}

void GameState::updateState() {
    auto log = getLogger("GameState");
    //loop through all objects
    std::map<std::string, std::shared_ptr<Object>>::iterator it = this->gameObjects.begin();
    float x;
    float y;
    float z;
    while (it != this->gameObjects.end()) {
        // check for collisions
        
        if (std::dynamic_pointer_cast<MovingObject>(it->second) != NULL) {           
            std::shared_ptr<MovingObject> temp = std::dynamic_pointer_cast<MovingObject>(it->second);
            checkCollisions(it->first, temp);
            x = temp->getNextPositionCollisionX();
            y = temp->getNextPositionCollisionY();
            z = temp->getNextPositionCollisionZ();
        }
        else{
        // calculate next position
            x = it->second->getNextPositionX();
            y = it->second->getNextPositionY();
            z = it->second->getNextPositionZ();
        }

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
    log->info("Finished updating state");
}

void GameState::applyEvent(std::shared_ptr<Event> event) {
    auto log = getLogger("GameState");
    std::map<std::string, std::shared_ptr<Object>>::iterator it;
    switch (event->getType()) {
    case Event::EventType::OEvent:
        log->info("Applying an Object Event: {}", event->serialize());
        it = gameObjects.find(event->getObjectId());
        if (it != gameObjects.end()) {
            std::dynamic_pointer_cast<ObjectEvent>(event)->apply(it->second);
            setDirty(true);
        }
        break;
    case Event::EventType::GEvent:
        log->info("Applying a GameState Event: {}", std::dynamic_pointer_cast<GameStateEvent>(event)->serialize());
        std::dynamic_pointer_cast<GameStateEvent>(event)->apply(this);
        break;
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
        std::shared_ptr<Object> obj = std::shared_ptr<Object>(new Player("cube4",0.0f,0.0f,3.0f,0.0f,0.0f,1.0f, 1.0f, 1.0f, 1.0f, 0.0f,0.0f,0.0f));
        std::cout << "INITIALIZING DEFAULT" << std::endl;
        this->createObject(obj, obj->getId());
        std::shared_ptr<Object> obj2 = std::shared_ptr<Object>(new Player("cube5", 3.0f, 0.0f, 3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f));
        std::cout << "INITIALIZING CUBE2" << std::endl;
        this->createObject(obj2, obj->getId());
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
        }
        it++;
    }
    res = res + "|";
    log->trace("Updates: {}", res);
    return res;
}

bool GameState::isDirty() {
    return this->dirty;
}

void GameState::setDirty(bool dty) {
    auto it = this->gameObjects.begin();
    while (it != this->gameObjects.end()) {
        it->second->dirty = false;
        it++;
    }
    this->dirty = dty;
}

void GameState::checkCollisions(std::string id, std::shared_ptr<MovingObject> object) {
    auto log = getLogger("GameState");
    auto it = this->gameObjects.begin();
    while (it != this->gameObjects.end()) {
        if (id != it->first) {
            std::shared_ptr<Object> currObj = it->second; //TODO make sure this resolves to MovingObject if currObj actually moves
            if (object->collidesNext(*currObj)) {
                object->handleXCollision(*currObj);
                object->handleYCollision(*currObj);
                object->handleZCollision(*currObj);
            }

        }
        it++;
        
    }
    log->trace("Finished checking collisions");
}

std::shared_ptr<Object> GameState::getObject(std::string id) {
    std::map<std::string, std::shared_ptr<Object>>::iterator it = gameObjects.find(id);
    if (it != gameObjects.end()) {
        return it->second;
    }
    return nullptr;
}