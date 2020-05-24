#include <limits>

#include <logger.h>

#include "Clients.h"

static constexpr unsigned int MAX_CLIENTS = 10;

static const auto LOGGER = getLogger( "Clients" );

/* Constructor and destructor */

Clients::Clients( const std::string & port ) : 
    inQueue( std::make_shared<BlockingQueue<Eptr>>() ), listenSock( makeListenSocket( port ) ), 
    connectionHandler( [this]() -> bool { return this->handleConnection(); } ),
    janitor( [this]() -> bool {

        std::shared_ptr<Client> next;
        trash.pop( next ); // Clear out next closed connection
        if ( next != nullptr ) LOGGER->debug( "Clearing client '{}'.", next->getID() );
        return next != nullptr; // Close on stop signal

    } ) {

    connectionHandler.start();
    connectionHandler.waitReady();

    janitor.start();

}

Clients::~Clients() {

    connectionHandler.stop();
    closesocket( listenSock ); // Close socket
    connectionHandler.waitDone();
    clients.clear();

    // Wait for trash to get emptied
    trash.push( nullptr );
    janitor.waitDone();

    LOGGER->info( "All connections closed." );

}

/* Public methods */

void Clients::broadcast( const Eptr & e ) {

    for ( auto it = clients.begin(); it != clients.end(); it++ ) {

        it->second->send( e );

    }

}

void Clients::send( const std::string & id, const Eptr & e ) {

    clients.at( id )->send( e );

}

bool Clients::receive( Eptr & dest, const unsigned long timeout ) {

    return inQueue->pop( dest, timeout );

}

bool Clients::tryReceive( Eptr & dest ) {

    return inQueue->tryPop( dest );

}

void Clients::receiveAll( std::deque<Eptr> & dest ) {

    inQueue->popAll( dest );

}

/* Protected methods */

std::string Clients::assignID( const std::string & request ) {

    std::lock_guard<std::mutex> lck( mtx );
    if ( !request.empty() && clients.count( request ) == 0 ) {
        clients[request] = nullptr;
        return request;
    }

    for ( unsigned int i = 0; i < std::numeric_limits<unsigned int>::max(); i++ ) {

        std::string name = std::string( "client-" ) + std::to_string( i );
        if ( clients.count( name ) == 0 ) {
            clients[name] = nullptr;
            return name;
        }

    }

    LOGGER->warn( "Could not find available name." );
    return "client-?";

}

/* Private methods */

SOCKET Clients::makeListenSocket( const std::string & port ) {

    struct addrinfo * result = NULL;
    struct addrinfo hints;

    memset( &hints, 0, sizeof( hints ) );
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve server address and port
    int status;
    if ( ( status = getaddrinfo( NULL, port.c_str(), &hints, &result ) ) != 0 ) {
        LOGGER->error( "getaddrinfo failed with error: {}", status );
        throw std::runtime_error( "Failed to resolve listen address." );
    }

    SOCKET listenSock = INVALID_SOCKET;

    // Create socket to listen for incoming connections
    listenSock = socket( result->ai_family, result->ai_socktype, result->ai_protocol );
    if ( listenSock == INVALID_SOCKET ) {
        LOGGER->critical( "Socket creation failed with error: {0:d}", WSAGetLastError() );
        freeaddrinfo( result );
        throw std::runtime_error( "Failed to create socket." );
    }

    // Bind connection listener socket
    if ( bind( listenSock, result->ai_addr, ( int ) result->ai_addrlen ) == SOCKET_ERROR ) {
        LOGGER->critical( "Failed to bind to socket with error: {0:d}.", WSAGetLastError() );
        freeaddrinfo( result );
        closesocket( listenSock );
        throw std::runtime_error( "Failed to bind to socket." );
    }

    freeaddrinfo( result );

    if ( listenSock == INVALID_SOCKET ) { // Should never happen
        LOGGER->critical( "Invalid socket." );
        throw std::runtime_error( "Invalid socket." );
    }

    // Listen on socket
    if ( listen( listenSock, SOMAXCONN ) == SOCKET_ERROR ) {
        LOGGER->critical( "Listen failed with error: {0:d}", WSAGetLastError() );
        closesocket( listenSock );
        throw std::runtime_error( "Failed to listen on socket." );
    }

    LOGGER->info( "Listening on port {}.", port );
    return listenSock;

}

bool Clients::handleConnection() {

    SOCKET clientSock = accept( listenSock, NULL, NULL );
    LOGGER->trace( "Received connection." );
    if ( clientSock == INVALID_SOCKET ) {
        LOGGER->error( "Failed to accept connection: {0:d}", WSAGetLastError() );
        return true;
    }

    if ( clients.size() >= MAX_CLIENTS ) {
        LOGGER->warn( "Refusing connection due to reaching client capacity." );
        closesocket( clientSock );
        return true;
    }

    std::shared_ptr<Client> client = std::make_shared<Client>( clientSock, inQueue,
                                                               [this]( const std::string & request ) -> std::string { return this->assignID( request ); },
                                                               [this]( const std::string & id ) -> void {
                                                                   auto it = clients.find( id );
                                                                   if ( it != clients.end() ) {
                                                                       std::shared_ptr<Client> client = it->second;
                                                                       clients.erase( id );
                                                                       trash.push( client ); // Store somewhere to avoid calling destructor on this thread
                                                                   } else {
                                                                       LOGGER->debug( "Client '{}' was already removed.", id );
                                                                   }
                                                               } );
    
    {
        std::lock_guard lck( mtx );
        clients[client->getID()] = client;
    }
    LOGGER->debug( "Registered client '{}'", client->getID() );
    return true;

}