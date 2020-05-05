#pragma once

#include <concurrent_queue.h>
#include "client.h"

class ConnectionsHandler {
private:
	// array of client objects (updated by original thread)
	Client** clients; // TODO use private array that gets updated from signals
	concurrency::concurrent_queue<int>* eventQueue;
	concurrency::concurrent_queue<int>* signalQueue;
public:
	ConnectionsHandler(Client**, concurrency::concurrent_queue<int>*, concurrency::concurrent_queue<int>*);
	bool tryPopEvent(int&);
	bool tryPopSignal(int&);
	int sendClient(int, char*, int);
};