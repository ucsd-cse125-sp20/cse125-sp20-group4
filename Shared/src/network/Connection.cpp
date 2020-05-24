#include <stdexcept>
#include <vector>

#include "logger.h"
#include "network/Connection.h"
#include "network/ConnectionClosedException.h"
#include "util/Sanitizer.h"

/* Size of the receive buffer */
static constexpr int BUFLEN = BUFSIZ;

static constexpr char START_MSG = '^';
static constexpr char END_MSG = '$';

static const auto LOGGER = getLogger( "Connection" );

/* Static helpers */

static std::vector<Sanitizer::CharMapping> specialCharacters() {

    std::vector<Sanitizer::CharMapping> charMap;

    ADD_CHAR_MAPPING( charMap, START_MSG, "crt" );
    ADD_CHAR_MAPPING( charMap, END_MSG, "dlr" );

    return charMap;

}

/* Used to sanitize messages */
static const Sanitizer sanitizer( specialCharacters() );

/* Constructor and destructor */

Connection::Connection( SOCKET sock ) : sock( sock ) {}

Connection::~Connection() {

    closesocket( sock );

}

/* Public interface */

void Connection::send( const std::string & message ) const {

    LOGGER->trace( "Sending message: '{}'", message );
    
    std::string buf;
    insertMessage( message, buf );
    int sent = ::send( sock, buf.c_str(), ( int ) buf.size(), 0 );
    if ( sent == SOCKET_ERROR ) {
        int error = WSAGetLastError();
        LOGGER->error( "Failed to send data: {}", error );
        throw std::runtime_error("Failed to send data.");
    } else if ( sent < buf.size() ) {
        LOGGER->warn( "Only sent {} out of {} bytes!", sent, buf.size() );
    }

}

std::string Connection::receive() {

    std::string message;
    unsigned int errorCount = 0;
    do {

        //LOGGER->trace( "Listening for more messages." );
        char inbuf[BUFLEN];
        int received = ::recv( sock, inbuf, BUFLEN, 0 );
        if ( received == SOCKET_ERROR ) { // Error
            int error = WSAGetLastError();
            LOGGER->error( "Failed to receive data: {}", error );
            throw std::runtime_error( "Failed to receive data." );
        } else if ( received == 0 ) { // Connection closed
            throw ConnectionClosedException();
        } else {
            std::string in_str( inbuf, received );
            inboundBuffer += in_str;
        }
        //LOGGER->trace( "Buffer: {}", inboundBuffer );

    } while ( !extractMessage( inboundBuffer, message ) );
    LOGGER->trace( "Received message: '{}'", message );
    return message;

}

/* Internal helpers */

void  Connection::insertMessage( const std::string & message, std::string & buffer ) {

    buffer += START_MSG + sanitizer.sanitize( message ) + END_MSG;

}

#define MESSAGE_START( pos ) ( ( pos ) + 1 )
#define MESSAGE_END( pos ) ( ( pos ) + 1 )

bool  Connection::extractMessage( std::string & buffer, std::string & message ) {

    size_t startPos = buffer.find( START_MSG ); // Find message start
    if ( startPos != 0 ) {
        LOGGER->warn( "Unstarted data received, ignoring: '{}'", buffer.substr( 0, startPos ) );
    }
    if ( startPos == std::string::npos ) {
        message = "";
        buffer = ""; // All data ignored
        return false;
    }

    size_t endPos = buffer.find( END_MSG, MESSAGE_START( startPos ) );

    size_t otherStart = buffer.find( START_MSG, MESSAGE_START( startPos ) );
    while ( otherStart < endPos ) { // Check for multiple starts before an end
        LOGGER->warn( "Unterminated data received, ignoring: '{}'", buffer.substr( startPos, otherStart - startPos ) );
        startPos = otherStart;
        otherStart = buffer.find( START_MSG, MESSAGE_START( startPos ) );
    }

    if ( endPos == std::string::npos ) { // Not a complete message
        message = "";
        buffer = buffer.substr( startPos );
        return false;
    }

    startPos = MESSAGE_START( startPos ); // Skip start marker
    message = sanitizer.restore( buffer.substr( startPos, endPos - startPos ) );
    buffer = buffer.substr( MESSAGE_END( endPos ) ); // Keep trailing content
    return true;

}
