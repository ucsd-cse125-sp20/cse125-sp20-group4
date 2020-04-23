#include "client.h"

Client::Client(int id, SOCKET sock, HANDLE thread, concurrency::concurrent_queue<int>* eventQueue) : id(id), sock(sock), thread(thread), eventQueue(eventQueue) {}

Client::~Client() {
    closesocket(sock);
    CloseHandle(thread);
}

HANDLE Client::getHandle() {
    return thread;
}

int Client::getId() {
    return id;
}

int Client::send(char* buf, int count) {
    return ::send(this->sock, buf, count, 0);
}

int Client::recv(char* buf, int count) {
    return ::recv(this->sock, buf, count, 0);
}

void Client::pushEvent(const int& src) {
    eventQueue->push(src);
}

void Client::pushEvent(int&& src) {
    eventQueue->push(src);
}

bool Client::tryPopEvent(int& dst) {
    return eventQueue->try_pop(dst);
}
