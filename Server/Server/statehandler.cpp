#include "statehandler.h"

GameState GameStateHandler::getNextState(GameState& gameState, concurrency::concurrent_queue<Event&>& eventQueue) {
    Event& cur;
    while (!eventQueue.empty()) { 
        cur = eventQueue.try_pop(); //we don't want to pop, just mark, so events have to have a way of marking them
        gameState.applyEvent(cur); 
    }
    //check integrity
    return gameState.updateState(); //this has integrity checks in it
}