#include "client.h"

Client::Client(int id, SOCKET* sock, concurrency::concurrent_queue<int>* eventQueue) {
    this->id = id;
    this->sock = sock;
    this->eventQueue = eventQueue;
}

Client::~Client() {
    closesocket(*sock);
}
