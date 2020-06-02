#ifndef CLIENTS_H
#define CLIENTS_H

#include <mutex>
#include <unordered_map>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <EventClasses/event.h>
#include <util/BlockingQueue.hpp>
#include <util/ThreadWorker.h>

#include "Client.h"

/**
 * Manages client connections to the server.
 */
class Clients {

    public:
    using Eptr = std::shared_ptr<Event>;
    using Qptr = std::shared_ptr<BlockingQueue<Eptr>>;

    /**
     * Initializes a manager that uses the given port to listen for new connections.
     *
     * @param port The port to listen to for new connections.
     */
    Clients( const std::string & port );

    /**
     * Closes all managed connections.
     */
    ~Clients();

    /**
     * Retrieves a client.
     *
     * @param id The ID of the client.
     * @return The client, or nullptr if there is no client with that ID.
     */
    std::shared_ptr<Client> getClient( const std::string & id ) const;

    /**
     * Retrieves the number of clients currently connected to the server.
     *
     * @return The number of connected clients.
     */
    unsigned int getClientCount() const;

    /**
     * Sends the given event to all clients.
     *
     * @param e The event to send.
     * @throws std::invalid_argument if the given event is null.
     */
    void broadcast( const Eptr & e );

    /**
     * Sends the given event to a specific client.
     *
     * @param e The event to send.
     * @throws std::invalid_argument if the given event is null.
     */
    void send( const std::string & id, const Eptr & e );

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
     * Assigns a client ID.
     *
     * @param request The preferred ID.
     * @return The assigned ID.
     */
    std::string assignID( const std::string & request );

    std::unordered_map<std::string, std::shared_ptr<Client>> clients;

    private:
    /**
     * Creates the server socket.
     *
     * @param port The port to listen to.
     * @return The opened socket.
     */
    static SOCKET makeListenSocket( const std::string & port );

    /**
     * Handles incoming connections.
     */
    bool handleConnection();

    const Qptr inQueue;
    const SOCKET listenSock;
    ThreadWorker connectionHandler;
    std::mutex mtx;

    // Destruct clients in dedicated thread to avoid self-join issues
    BlockingQueue<std::shared_ptr<Client>> trash;
    ThreadWorker janitor;

};
#endif