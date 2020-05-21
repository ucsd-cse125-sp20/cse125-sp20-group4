#include <logger.h>

#include "server.h"

const auto LOGGER = getLogger( "Server" );

Server::Server(SOCKET sock, concurrency::concurrent_queue<std::shared_ptr<Event>>* eventQueue) : sock(sock), eventQueue(eventQueue) {}

Server::~Server() {
	closesocket(sock);
}

int Server::send(char* buf, size_t count) {
	return ::send(this->sock, buf, (int)count, 0);
}

int Server::recv(char* buf, size_t count) {
	return ::recv(this->sock, buf, (int)count, 0);
}

void Server::pushEvent( const std::shared_ptr<Event> & src ) {
	
	//eventQueue->push(src);

	// While there's nothing handling the queue...
	std::string serialized = src->serialize();
	LOGGER->trace( "Serialized event: {}", serialized );
	send( &serialized[0], serialized.length() );

}

bool Server::tryPopEvent(std::shared_ptr<Event>& dst) {
	return eventQueue->try_pop(dst);
}

