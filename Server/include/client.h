#pragma once

#include <winsock2.h>
#include <processthreadsapi.h>
#include <concurrent_queue.h>
#include "EventClasses/event.h"
#include "client.h"


class Client {
private: // todo private
	int id;
	SOCKET sock;
	HANDLE thread = NULL;
	concurrency::concurrent_queue<int>* eventQueue; //todo use shared data class
public:
	Client(int, SOCKET, HANDLE, concurrency::concurrent_queue<std::shared_ptr<Event>>*);
	~Client();

	// getters
	HANDLE getHandle();
	int getId();

	int send(char*, unsigned int); // todo send shared data type, include serialization inside
	int recv(char*, unsigned int); // todo recv shared data type, include deserialization inside
	void pushEvent(const std::shared_ptr<Event>&); // todo push shared data type
	void pushEvent(std::shared_ptr<Event>&&); // todo push shared data type
	bool tryPopEvent(std::shared_ptr<Event>&); // todo push shared data type
};