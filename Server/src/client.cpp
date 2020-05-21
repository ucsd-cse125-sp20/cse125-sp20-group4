#define GAMESTATE_SEND_BUFFER_SIZE 4096
#include "client.h"

Client::Client(int id, SOCKET sock, HANDLE thread, concurrency::concurrent_queue<std::shared_ptr<Event>>* eventQueue) : id(id), sock(sock), thread(thread), eventQueue(eventQueue) {}

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

int Client::send(char* buf, size_t count) {
    return ::send(this->sock, buf, (int)count, 0);
}

int Client::recv(char* buf, size_t count) {
    return ::recv(this->sock, buf, (int)count, 0);
}

void Client::pushEvent(const std::shared_ptr<Event>& src) {
    eventQueue->push(src);
}
/*
void Client::pushEvent(std::shared_ptr<Event>&& src) {
    eventQueue->push(src);
}*/

bool Client::tryPopEvent(std::shared_ptr<Event>& dst) {
    return eventQueue->try_pop(dst);
}

bool Client::sendGameState(GameState& gs){
    std::string serialized = gs.serialize();
    //std::string serialized = "";
    //const char* buf = serialized.c_str();
    //char* yeet = buf;
    this->send(&serialized[0], serialized.length());
    return true; //TODO make this dependent on send result
}