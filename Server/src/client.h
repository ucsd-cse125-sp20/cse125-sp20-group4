#pragma once

#include <winsock2.h>
#include <processthreadsapi.h>
#include <concurrent_queue.h>

class Client {
private: // todo private
	int id;
	SOCKET sock;
	HANDLE thread = NULL;
	concurrency::concurrent_queue<int>* eventQueue; //todo use shared data class
public:
	Client(int, SOCKET, HANDLE, concurrency::concurrent_queue<int>*);
	~Client();

	// getters
	HANDLE getHandle();
	int getId();

	int send(char*, int); // todo send shared data type, include serialization inside
	int recv(char*, int); // todo recv shared data type, include deserialization inside
	void pushEvent(const int&); // todo push shared data type
	void pushEvent(int&&); // todo push shared data type
	bool tryPopEvent(int&); // todo push shared data type
};