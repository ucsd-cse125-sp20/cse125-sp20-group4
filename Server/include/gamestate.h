#pragma once
#include <map>
#include <string>
#include <iostream>
#include "ObjectClasses/player.h"
#include "EventClasses/event.h"
#include <memory>
class GameState
{
private:
    std::map<std::string, std::shared_ptr<Object>> gameObjects;
    int nextId;
public:
    GameState();
    void createObject(std::shared_ptr<Object> obj);
    void createObject(std::shared_ptr<Object> obj, std::string id);
    void deleteObject(std::string id);
    void updateState();
    void applyEvent(Event* event);
    std::string serialize();
};