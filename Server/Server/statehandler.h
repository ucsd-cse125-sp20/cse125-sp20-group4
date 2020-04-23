#pragma once
#include <queue>
#include "ObjectClasses/object.h"
#include "gamestate.h"
#include "EventClasses/event.h"
#include <concurrent_queue.h>

class GameStateHandler
{
public:
    void getNextState(GameState& gameState, concurrency::concurrent_queue<Event&>& eventQueue);
};