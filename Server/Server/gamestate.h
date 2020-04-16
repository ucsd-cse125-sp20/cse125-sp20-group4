#pragma once
#include <map>
#include "ObjectClasses/player.h"
using namespace std;

class GameState
{
private:
    map<int, Object> gameObjects;
    map<int, Player> players
public:
    GameState();
    updateObject(int id);
    createObject(Object obj);
    deleteObject(int id);
};