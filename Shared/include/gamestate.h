#pragma once
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include "EventClasses/event.h"
#include "ObjectClasses/player.h"
class GameState
{
private:
    std::map<std::string, std::shared_ptr<Object>> gameObjects;
    int nextId;
    bool dirty;

public:
    GameState();
    void createObject(std::shared_ptr<Object> obj);
    void createObject(std::shared_ptr<Object> obj, std::string id);
    std::shared_ptr<Object> getObject(std::string id);
    void deleteObject(std::string id);
    void updateState();
    void applyEvent(std::shared_ptr<Event> event);
    std::string serialize();
    void initialize(std::string file = "");
    std::string getUpdates();
    bool isDirty();
    void setDirty(bool dty);
    void checkCollisions(std::string id, std::shared_ptr<MovingObject> object);
    const std::map<std::string, std::shared_ptr<Object>> & getGameObjects();

};