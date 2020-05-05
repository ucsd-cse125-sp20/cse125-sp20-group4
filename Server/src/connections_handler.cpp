#include "connections_handler.h"

ConnectionsHandler::ConnectionsHandler(Client** clients, concurrency::concurrent_queue<int>* eventQueue, concurrency::concurrent_queue<int>* signalQueue) :
	clients(clients),
	eventQueue(eventQueue),
	signalQueue(signalQueue) {}

bool ConnectionsHandler::tryPopEvent(int& dst) {
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