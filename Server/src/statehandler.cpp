#include "statehandler.h"

void GameStateHandler::getNextState(GameState& gameState, concurrency::concurrent_queue<Event&>& eventQueue) {
    Event e = Event(1);
    Event& cur = e;//dummy
    while (!eventQueue.empty()) { 
        bool success = eventQueue.try_pop(cur); //we don't want to pop, just mark, so events have to have a way of marking them
        gameState.applyEvent(cur); 
    }
    //check integrity
    gameState.updateState(); //this has integrity checks in it
}