#pragma once

#include <winsock2.h>
#include <processthreadsapi.h>
#include <concurrent_queue.h>

class Client {
public: // todo private
	int id;
	SOCKET* sock;
	concurrency::concurrent_queue<int>* eventQueue; //todo use shared data class
	concurrency::concurrent_queue<int> signalQueue = concurrency::concurrent_queue<int>();
public:
	Client(int id, SOCKET* sock, concurrency::concurrent_queue<int>* eventQueue);
	~Client();
};