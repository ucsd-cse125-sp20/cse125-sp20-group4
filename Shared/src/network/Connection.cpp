#include <regex>
#include <stdexcept>

#include "logger.h"
#include "network/Connection.h"
#include "network/ConnectionClosedException.h"

/* Size of the receive buffer */
constexpr int BUFLEN = BUFSIZ;

/* Character used to mark the start of a special expression */
constexpr char MARKER_CHAR = '&';
#define SEQUENCE( s ) ( MARKER_CHAR + std::string( s ) + ';' )

static const std::string MARKER = std::string() + MARKER_CHAR;
static const std::string MARKER_SEQ = SEQUENCE( "amp" );
static const std::string BEGIN_SEQ = SEQUENCE( "begin" );
static const std::string END_SEQ = SEQUENCE( "end" );

static const std::regex MARKER_REGEX( std::string() + MARKER );
static const std::regex MARKER_SEQ_REGEX( MARKER_SEQ );

static const auto LOGGER = getLogger( "Connection" );

/* Constructor and destructor */

Connection::Connection( SOCKET sock ) : sock( sock ) {}

Connection::~Connection() {

    closesocket( sock );

}

/* Public interface */

void Connection::send( const std::string & message ) const {

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

        char inbuf[BUFLEN];
        int received = ::recv( sock, inbuf, BUFLEN, 0 );
        if ( received == SOCKET_ERROR ) { // Error
            int error = WSAGetLastError();
            spdlog::error( "Failed to receive data: {}", error );
            throw std::runtime_error( "Failed to receive data." );
        } else if ( received == 0 ) { // Connection closed
            throw ConnectionClosedException();
        } else {
            std::string in_str( inbuf, received );
            inboundBuffer += in_str;
        }

    } while ( !extractMessage( inboundBuffer, message ) );
    return message;

}

/* Internal helpers */

std::string Connection::sanitize( const std::string & message ) {

    return std::regex_replace( message, MARKER_REGEX, MARKER_SEQ );

}

std::string Connection::restore( const std::string & sanitized ) {

    return std::regex_replace( sanitized, MARKER_SEQ_REGEX, MARKER );

}

void  Connection::insertMessage( const std::string & message, std::string & buffer ) {

    buffer += BEGIN_SEQ + sanitize( message ) + END_SEQ;

}

bool  Connection::extractMessage( std::string & buffer, std::string & message ) {

    size_t startPos = buffer.find( BEGIN_SEQ ); // Find message start
    if ( startPos == std::string::npos ) { // No message started
        size_t markerPos = buffer.rfind( MARKER_CHAR );
        if ( markerPos != std::string::npos && buffer.size() - markerPos < BEGIN_SEQ.size() && BEGIN_SEQ.find( buffer.substr( markerPos ) ) != std::string::npos ) {
            startPos = markerPos; // May be a partial start tag, don't ignore
        }
    }
    if ( startPos != 0 ) {
        LOGGER->warn( "Unstarted data received, ignoring: {}", buffer.substr( 0, startPos ) );
    }
    if ( startPos == std::string::npos ) {
        message = "";
        buffer = ""; // All data ignored
        return false;
    }

    size_t endPos = buffer.find( END_SEQ, startPos );

    size_t otherStart = buffer.find( BEGIN_SEQ, startPos );
    while ( otherStart < endPos ) { // Check for multiple starts before an end
        LOGGER->warn( "Unterminated data received, ignoring: {}", buffer.substr( startPos, otherStart - startPos ) );
        startPos = otherStart;
        otherStart = buffer.find( BEGIN_SEQ, startPos );
    }

    if ( endPos == std::string::npos ) { // Not a complete message
        message = "";
        buffer = buffer.substr( startPos );
        return false;
    }

    startPos += BEGIN_SEQ.size(); // Skip start marker
    message = restore( buffer.substr( startPos, endPos - startPos ) );
    buffer = buffer.substr( endPos + END_SEQ.size() ); // Keep trailing content
    return true;

}
