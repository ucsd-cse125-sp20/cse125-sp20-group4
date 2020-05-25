#pragma once

#include <EventClasses/event.h>
#include <network/Connection.h>
#include <network/EventConnection.h>
#include <util/BlockingQueue.hpp>

class Server {

    public:
    using Eptr = std::shared_ptr<Event>;

    Server( const std::string & address, const std::string & port );

    /**
     * Retrieves the underlying connection to the server.
     *
     * @return The connection.
     */
    EventConnection & connection();

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
     */
	static void setup( const std::shared_ptr<Connection> & conn, const std::shared_ptr<BlockingQueue<Eptr>> & inQueue );

    /**
     * Performs any cleanup necessary after the connection stopped being used but is not closed yet.
     *
     * @param conn The connection.
     * @param inQueue The inbound event queue.
     */
	static void teardown( const std::shared_ptr<Connection> & conn, const std::shared_ptr<BlockingQueue<Eptr>> & inQueue );

	private:
	EventConnection conn;

};