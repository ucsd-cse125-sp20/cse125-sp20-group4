#pragma once
#include <map>
#include <string>
#include <iostream>
#include "ObjectClasses/player.h"

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
    std::string serialize();
};