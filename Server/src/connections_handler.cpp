#include "connections_handler.h"
#define MAX_CLIENTS 5

ConnectionsHandler::ConnectionsHandler(Client** clients, concurrency::concurrent_queue<std::shared_ptr<Event>>* eventQueue, concurrency::concurrent_queue<int>* signalQueue) :
	clients(clients),
	eventQueue(eventQueue),
	signalQueue(signalQueue) {}

bool ConnectionsHandler::tryPopEvent(std::shared_ptr<Event>& dst) {
	return eventQueue->try_pop(dst);
}

bool ConnectionsHandler::tryPopSignal(int& dst) {
	return signalQueue->try_pop(dst);
}

int ConnectionsHandler::sendClient(int clientId, char* buf, int count) {
	// WARNING: potential bad concurrency reference if client deleted between check and use
	// TODO: main thread can only delete client if game thread already removed reference (signal -> wait response -> delete)
	if (clients[clientId]) {
		return clients[clientId]->send(buf, count);
	} else {
		return -1;
	}
}

concurrency::concurrent_queue<std::shared_ptr<Event>>* ConnectionsHandler::getEventQueue()
{
	return this->eventQueue;
}

bool ConnectionsHandler::sendGameStateToAll(GameState& gs) {
	for (int i = 0; i < MAX_CLIENTS; i++) {
		if (clients[i] != NULL) {
			clients[i]->sendGameState(gs);
		}
	}
	return true;
}

