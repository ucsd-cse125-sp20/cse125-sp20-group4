#include <regex>
#include <stdexcept>

#include "logger.h"
#include "network/Connection.h"

constexpr int BUFLEN = 512;
constexpr unsigned int MAX_ERRORS = 5;

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

void Connection::send( const std::string & message ) {

    std::string buf;
    insertMessage( message, buf );
    int sent = std::send( sock, buf.c_str(), buf.size(), 0 );
    if ( sent == SOCKET_ERROR ) {
        int error = WSAGetLastError();
        LOGGER->error( "Failed to send data: {}", error );
    } else if ( sent < buf.size() ) {
        LOGGER->warn( "Only sent {} out of {} bytes!", sent, buf.size() );
    }

}

std::string Connection::receive() {

    std::string message;
    unsigned int errorCount = 0;
    do {

        char inbuf[DEFAULT_BUFLEN];
        int received = server->recv( inbuf, BUFLEN );
        if ( received == SOCKET_ERROR ) { // Error
            int error = WSAGetLastError();
            spdlog::error( "Failed to receive data: {}", error );
            if ( errorCount++ == MAX_ERRORS ) {
                throw std::runtime_error( "Exceeded maximum receive errors." );
            }
        } else if ( received == 0 ) { // Connection closed
            // TODO
        } else {
            std::string in_str( inbuf, received );
            spdlog::trace( "Received message from server: {0}", in_str ); //only for testing, can be removed

            // todo push game state onto event queue
            std::string message = leftover + in_str;
            spdlog::trace( "Full message: {0}", message ); //only for testing, can be removed
            leftover = deserializer.deserializeUpdates( message, updates );
            spdlog::trace( "Message leftover: {}", leftover );
            if ( !updates->empty() ) {
                //@Thiago this pointer to unordered map gives you ID to object mappings, and objects give you position/direction TODO
                spdlog::debug( "Number of updates: {}", updates->size() );
                Window::world->handleUpdates( updates );
                updates->clear();
            }
        }

    } while ( !extractMessage( inboundBuffer, message ) );

}

/* Internal helpers */

std::string Connection::sanitize( const std::string & message ) {

    return std::regex_replace( message, MARKER_REGEX, MARKER_SEQ );

}

std::string Connection::restore( const std::string & sanitized ) {

    return std::regex_replace( sanitized, MARKER_SEQ_REGEX, MARKER );

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

void  Connection::insertMessage( const std::string & message, std::string & buffer ) {

    out += BEGIN_SEQ + sanitize( message ) + END_SEQ;

}