#pragma once

#include <deque>

#include "EventClasses/event.h"
#include "ObjectClasses/object.h"

#include "gamestate.h"

class GameStateHandler {

    public:
    void getNextState( GameState * gameState, std::deque<std::shared_ptr<Event>> * eventQueue );

};