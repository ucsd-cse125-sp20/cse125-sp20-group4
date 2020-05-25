#include "network/EventConnection.h"

void EventConnection::encode( const Tptr & src, std::string & message ) {

    message = src->serialize();

}

void EventConnection::decode( const std::string & message, Tptr & dest ) {

    dest = dec.deserializeEvent( message );

}