#pragma once
#include <map>
#include <string>
#include <iostream>
#include "ObjectClasses/player.h"
#include "EventClasses/event.h"

class GameState
{
private:
    std::map<int, Object&> gameObjects;
    int nextId;
public:
    GameState();
    void updateObject(int id);
    void createObject(Object& obj);
    void deleteObject(int id);
    void updateState();
    void applyEvent(Event* event);
    std::string serialize();
};