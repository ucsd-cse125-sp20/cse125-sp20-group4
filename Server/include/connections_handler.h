#pragma once

#include <concurrent_queue.h>
#include "client.h"
#include "EventClasses/event.h"
#include "gamestate.h"

class ConnectionsHandler {
private:
	// array of client objects (updated by original thread)
	Client** clients; // TODO use private array that gets updated from signals
	concurrency::concurrent_queue<std::shared_ptr<Event>>* eventQueue;
	concurrency::concurrent_queue<int>* signalQueue;
public:
	ConnectionsHandler(Client**, concurrency::concurrent_queue<std::shared_ptr<Event>>*, concurrency::concurrent_queue<int>*);
	bool tryPopEvent(std::shared_ptr<Event>&);
	bool tryPopSignal(int&);
	int sendClient(int, char*, int);
	bool sendGameStateToAll(GameState& gs); //TODO change to shared ptr
	concurrency::concurrent_queue<std::shared_ptr<Event>>* getEventQueue();
};