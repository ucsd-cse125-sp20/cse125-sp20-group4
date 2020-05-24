#include "statehandler.h"

void GameStateHandler::getNextState( GameState * gameState, std::deque<std::shared_ptr<Event>> * eventQueue ) {

    std::shared_ptr<Event> cur;
    while ( !eventQueue->empty() ) {
        cur = eventQueue->front();
        eventQueue->pop_front(); //we don't want to pop, just mark, so events have to have a way of marking them
        gameState->applyEvent( cur ); 
    }

    //check integrity
    gameState->updateState(); //this has integrity checks in it
}