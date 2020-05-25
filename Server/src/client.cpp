#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdexcept>

#include <logger.h>

#include "Client.h"

const auto LOGGER = getLogger( "Client" );

/* Constructor and destructor */

Client::Client( SOCKET sock, const Qptr & inQueue, 
                std::function<std::string( const std::string & )> idAssigner,
                std::function<void( const std::string & )> closeCallback ) :
    conn( sock, inQueue,
          [this, idAssigner]( const Cptr & c, const Qptr & q ) -> void { this->setup( c, q, idAssigner ); },
          [this]( const Cptr & c, const Qptr & q ) -> void { this->teardown( c, q ); } ), 
    closeCallback( closeCallback ) {}

/* Protected methods */

void Client::setup( const Cptr & /* conn */, const Qptr & /* inQueue */, 
                    std::function<std::string( const std::string & )> idAssigner ) {

    // Do nothing
    std::string requestedName = "cube4"; // Try to use this name if possible

    // Store client ID
    clientID = idAssigner( requestedName );
    LOGGER->info( "Established connection with client '{}'.", clientID );

}

void Client::teardown( const Cptr & /* conn */, const Qptr & /* inQueue */ ) {

    // Do nothing

    LOGGER->info( "Closed connection with client '{}'.", clientID );
    
    closeCallback( clientID );

}

/* Public methods */

const std::string & Client::getID() const {

    return clientID;

}

EventConnection & Client::connection() {

    return conn;

}

void Client::send( const Eptr & e ) {

    conn.send( e );

}

bool Client::receive( Eptr & dest, const unsigned long timeout ) {

    return conn.receive( dest, timeout );

}

bool Client::tryReceive( Eptr & dest ) {

    return conn.tryReceive( dest );

}

void Client::receiveAll( std::deque<Eptr> & dest ) {

    conn.receiveAll( dest );

}
