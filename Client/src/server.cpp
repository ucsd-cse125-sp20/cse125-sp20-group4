#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdexcept>

#include <logger.h>

#include "Server.h"

const auto LOGGER = getLogger( "Server" );

/* Local helpers */

/**
 * Connects to the server on the given address and port.
 *
 * @param address The address of the server.
 * @param port The port of the server.
 */
static SOCKET connect( const std::string & address, const std::string & port ) {

    LOGGER->info( "Connecting to server on {}:{}", address, port );

    int status;
    struct addrinfo * result = NULL;
    struct addrinfo hints;

    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve server address and port
    if ( ( status = getaddrinfo( address.c_str(), port.c_str(), &hints, &result ) ) != 0 ) {
        LOGGER->critical( "getaddrinfo failed with error: {0:d}", status );
        throw std::runtime_error( "Failed to resolve server address." );
    }

    // Create socket to use
    SOCKET sock = INVALID_SOCKET;
    sock = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
    if ( sock == INVALID_SOCKET ) {
        LOGGER->critical( "Socket creation failed with error: {0:d}", WSAGetLastError() );
        freeaddrinfo( result );
        throw std::runtime_error( "Failed to create socket." );
    }

    // Connect to server
    if ( ( status = connect( sock, result->ai_addr, ( int ) result->ai_addrlen ) ) == SOCKET_ERROR ) {
        LOGGER->critical( "Failed to connect to server with error: {0:d}.", WSAGetLastError() );
        freeaddrinfo( result );
        closesocket( sock );
        throw std::runtime_error( "Failed to connect to server." );
    }

    freeaddrinfo( result );

    if ( sock == INVALID_SOCKET ) { // Should never happen
        LOGGER->critical( "Invalid socket." );
        throw std::runtime_error( "Invalid socket." );
    }

    LOGGER->info( "Connected to server." );
    return sock;

}

/* Constructor */

Server::Server( const std::string & address, const std::string & port ) : conn( connect( address, port ), setup, teardown ) {}

/* Protected methods */

void Server::setup( const std::shared_ptr<Connection> & /* conn */, const std::shared_ptr<BlockingQueue<Eptr>> & /* inQueue */ ) {

    // Do nothing

}

void Server::teardown( const std::shared_ptr<Connection> & /* conn */, const std::shared_ptr<BlockingQueue<Eptr>> & /* inQueue */ ) {

    // Do nothing

}

/* Public methods */

void Server::send( const Eptr & e ) {

    conn.send( e );

}

bool Server::receive( Eptr & dest, const unsigned long timeout ) {

    return conn.receive( dest, timeout );

}

bool Server::tryReceive( Eptr & dest ) {

    return conn.tryReceive( dest );

}

void Server::receiveAll( std::deque<Eptr> & dest ) {

    conn.receiveAll( dest );

}
