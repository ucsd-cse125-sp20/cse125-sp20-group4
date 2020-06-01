#ifndef CONNECTION_CLOSED_EXCEPTION_H
#define CONNECTION_CLOSED_EXCEPTION_H

#include <stdexcept>

/**
 * Exception that represents an attempted usage of a closed connection.
 */
class ConnectionClosedException : public std::runtime_error {

    public:
    ConnectionClosedException() : runtime_error( "Connection is already closed." ) {}

};

#endif