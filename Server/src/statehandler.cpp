#include "statehandler.h"
#include "logger.h"

void GameStateHandler::getNextState( GameState * gameState, std::deque<std::shared_ptr<Event>> * eventQueue ) {

    std::shared_ptr<Event> cur;
    while ( !eventQueue->empty() ) {
        cur = eventQueue->front();
        eventQueue->pop_front(); //we don't want to pop, just mark, so events have to have a way of marking them
        gameState->applyEvent( cur ); 
    }
    // update timers and call callbacks
    std::map<std::string, std::function<void()>> timerCallbacks = gameState->updateTimers();
    for (auto it = timerCallbacks.begin(); it != timerCallbacks.end(); it++) {
        it->second();
    }
    //check integrity
    gameState->updateState(); //this has integrity checks in it
}