#ifndef EVENT_CONNECTION_H
#define EVENT_CONNECTION_H

#include "deserializer.h"
#include "EventClasses/event.h"
#include "network/AsyncConnection.hpp"

/**
 * A connection interface that sends and receives events.
 */
class EventConnection : public AsyncConnection<Event> {

    public:
    /**
     * Creates a new connection that communicates over the given socket.
     *
     * @param sock The socket to communicate over.
     * @param inQueue The queue to store incoming events into.
     * @param setup The callback to execute after the connection is established but before
     *              asyncronous sending/receiving starts. If null, nothing is called.
     *              It is given the internal connection and the queue that incoming events
     *              are sent to.
     * @param teardown The callback to execute before asyncronous sending/receiving stops
     *                 but before the connection is closed. If null, nothing is called.
     *                 It is given the internal connection and the queue that incoming events
     *                 are sent to.
     */
    EventConnection( SOCKET sock, const Qptr & inQueue,
                     const Callback setup = nullptr,
                     const Callback teardown = nullptr ) :
        AsyncConnection( sock, inQueue, setup, teardown ) {}

    /**
     * Creates a new connection that communicates over the given socket.
     * A new queue is created for incoming events.
     *
     * @param sock The socket to communicate over.
     * @param setup The callback to execute after the connection is established but before
     *              asyncronous sending/receiving starts. If null, nothing is called.
     *              It is given the internal connection and the queue that incoming events
     *              are sent to.
     * @param teardown The callback to execute before asyncronous sending/receiving stops
     *                 but before the connection is closed. If null, nothing is called.
     *                 It is given the internal connection and the queue that incoming events
     *                 are sent to.
     */
    EventConnection( SOCKET sock,
                     const Callback setup = nullptr,
                     const Callback teardown = nullptr ) :
        AsyncConnection( sock, setup, teardown ) {}

    protected:
    // Override
    void encode( const Tptr & src, std::string & message );
    // Override
    void decode( const std::string & message, Tptr & dest );

    private:
    Deserializer dec;

};

#endif