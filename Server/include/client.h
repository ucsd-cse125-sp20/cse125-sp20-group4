#pragma once

#include <functional>

#include <EventClasses/event.h>
#include <network/Connection.h>
#include <network/EventConnection.h>
#include <util/BlockingQueue.hpp>

/**
 * Represents a client that is connected to the server.
 */
class Client {

    public:
    using Eptr = std::shared_ptr<Event>;
    using Qptr = std::shared_ptr<BlockingQueue<Eptr>>;
    using Cptr = std::shared_ptr<Connection>;

    /**
     * Initializes a client that is connected throught the given socket.
     *
     * @param sock The socket that the client is connected to.
     * @param inQueue The queue that inbound events should be sent to.
     * @param nameAssigner The function to call to assign an id to the client.
     *                     It takes the client's requested ID (or an empty string if no ID was
     *                     requested), and should return the assigned ID.
     * @param closeCallback The function to call after the client disconnects.
     */
    Client( SOCKET sock, const Qptr & inQueue, 
            std::function<std::string( const std::string & )> idAssigner, 
            std::function<void( const std::string & )> closeCallback );

    /**
     * Retrieves the ID of the client.
     *
     * @return The client ID.
     */
    const std::string & getID() const;

    /**
     * Queues the given event for sending.
     *
     * @param e The event to send.
     * @throws std::invalid_argument if the given event is null.
     */
    void send( const Eptr & e );

    /**
     * Retrieves the next available incoming event, blocking until one is available if
     * necessary.
     *
     * @param dest Where to store the retrieved event into.
     * @param timeout If not 0, the maximum amount of time to block for if
     *                no events are available.
     * @return True if an event was successfully retrieved, false if a timeout
     *         occurred. Will never be false if timeout is 0.
     */
    bool receive( Eptr & dest, const unsigned long timeout = 0 );

    /**
     * Retrieves the next available incoming event, if there is one. If there are no
     * available events, returns immediately.
     *
     * @param dest Where to store the retrieved event into.
     * @return True if an event was successfully retrieved, false if there are
     *         no events available.
     */
    bool tryReceive( Eptr & dest );

    /**
     * Retrieves all currently available incoming events.
     *
     * @param dest Where to place the events into.
     *             Elements are placed so that the oldest element is at
     *             the front of the queue.
     */
    void receiveAll( std::deque<Eptr> & dest );

    protected:
    /**
     * Performs any setup necessary after the connection is open but before it starts being used.
     *
     * @param conn The openened connection.
     * @param inQueue The inbound event queue.
     * @param idAssigner The function to call to assign an ID to the client.
     */
    void setup( const Cptr & conn, const Qptr & inQueue,
                std::function<std::string( const std::string & )> idAssigner );
    /**
     * Performs any cleanup necessary after the connection stopped being used but is not closed yet.
     *
     * @param conn The connection.
     * @param inQueue The inbound event queue.
     */
    void teardown( const Cptr & conn, const Qptr & inQueue );

    const std::function<void( std::string )> closeCallback;

    private:
    std::string clientID;
    EventConnection conn;

};