#include "statehandler.h"

void GameStateHandler::getNextState(GameState* gameState, concurrency::concurrent_queue<std::shared_ptr<Event>>* eventQueue) {
    std::shared_ptr<Event> cur;
    while (!eventQueue->empty()) { 
        eventQueue->try_pop(cur); //we don't want to pop, just mark, so events have to have a way of marking them
        gameState->applyEvent(cur); 
    }
    //check integrity
    gameState->updateState(); //this has integrity checks in it
}