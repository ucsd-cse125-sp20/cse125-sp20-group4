#pragma once
#include <winsock2.h>
#include <concurrent_queue.h>
#include "EventClasses/event.h"

class Server {
private: // todo
	SOCKET sock;
	concurrency::concurrent_queue<std::shared_ptr<Event>>* eventQueue; //todo replace int with shared data class
public:
	Server(SOCKET, concurrency::concurrent_queue<std::shared_ptr<Event>>*);
	~Server();

	int send(char*, size_t); //todo send shared data type, include serialization
	int recv(char*, size_t); //todo recv shared data type, include deserialization
	void pushEvent(const std::shared_ptr<Event>&); // todo push shared data type
	void pushEvent(std::shared_ptr<Event>&&); // todo push shared data type
	bool tryPopEvent(std::shared_ptr<Event>&); // todo push shared data type

};