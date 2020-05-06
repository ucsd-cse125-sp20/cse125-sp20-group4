#include <winsock2.h>
#include<concurrent_queue.h>

class Server {
private: // todo
	SOCKET sock;
	concurrency::concurrent_queue<int>* eventQueue; //todo replace int with shared data class
public:
	Server(SOCKET, concurrency::concurrent_queue<int>*);
	~Server();

	int send(char*, int); //todo send shared data type, include serialization
	int recv(char*, int); //todo recv shared data type, include deserialization
	void pushEvent(const int&); // todo push shared data type
	void pushEvent(int&&); // todo push shared data type
	bool tryPopEvent(int&); // todo push shared data type

};