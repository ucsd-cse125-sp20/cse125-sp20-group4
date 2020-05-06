#include "server.h"

Server::Server(SOCKET sock, concurrency::concurrent_queue<int>* eventQueue) : sock(sock), eventQueue(eventQueue) {}

Server::~Server() {
	closesocket(sock);
}

int Server::send(char* buf, int count) {
	return ::send(this->sock, buf, count, 0);
}

int Server::recv(char* buf, int count) {
	return ::recv(this->sock, buf, count, 0);
}

void Server::pushEvent(const int& src) {
	eventQueue->push(src);
}

void Server::pushEvent(int&& src) {
	eventQueue->push(src);
}

bool Server::tryPopEvent(int& dst) {
	return eventQueue->try_pop(dst);
}

