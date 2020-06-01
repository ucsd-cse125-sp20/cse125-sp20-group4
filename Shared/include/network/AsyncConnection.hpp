#ifndef ASYNC_CONNECTION_H
#define ASYNC_CONNECTION_H

#include <winsock2.h>
#include <ws2tcpip.h>

#include <functional>
#include <mutex>
#include <stdexcept>
#include <thread>

#include "logger.h"
#include "network/Connection.h"
#include "util/BlockingQueue.hpp"
#include "util/ThreadWorker.h"

/**
 * A connection interface that sends an receives objects through a socket.
 * All network operations (sending/receiving) are done in background threads.
 *
 * Asyncronous sending/receiving continues until either close() is called or
 * the peer signals a close, at which point async sending/receiving stops
 * and the underlying connection is closed.
 *
 * @tparam T Type of object being communicated over the network.
 */
template <class T>
class AsyncConnection {

    public:
    using Tptr = std::shared_ptr<T>;
    using Queue = BlockingQueue<Tptr>;
    using Qptr = std::shared_ptr<Queue>;
    using Callback = std::function<void( const std::shared_ptr<Connection> &, const Qptr & )> ;

    /**
     * Creates a new connection that communicates over the given socket.
     *
     * @param sock The socket to communicate over.
     * @param inQueue The queue to store incoming objects into.
     * @param setup The callback to execute after the connection is established but before
     *              asyncronous sending/receiving starts. If null, nothing is called.
     *              It is given the internal connection and the queue that incoming objects
     *              are sent to.
     * @param teardown The callback to execute before asyncronous sending/receiving stops
     *                 but before the connection is closed. If null, nothing is called.
     *                 It is given the internal connection and the queue that incoming objects
     *                 are sent to.
     */
    AsyncConnection( SOCKET sock, const Qptr & inQueue, 
                     const Callback setup = nullptr,
                     const Callback teardown = nullptr ) :
        sender( [this]() -> bool { return this->senderLoop(); } ),
        receiver( [this]() -> bool { return this->receiverLoop(); } ),
        outQueue( std::make_shared<Queue>() ), inQueue( inQueue ),
        conn( std::make_shared<Connection>( sock ) ),
        stopper( nullptr ), teardown( teardown ) {
    
        // Execute setup callback
        if ( setup != nullptr ) {
            setup( conn, inQueue );
        }
    
    }

    /**
     * Creates a new connection that communicates over the given socket.
     * A new queue is created for incoming objects.
     *
     * @param sock The socket to communicate over.
     * @param setup The callback to execute after the connection is established but before
     *              asyncronous sending/receiving starts. If null, nothing is called.
     *              It is given the internal connection and the queue that incoming objects
     *              are sent to.
     * @param teardown The callback to execute before asyncronous sending/receiving stops
     *                 but before the connection is closed. If null, nothing is called.
     *                 It is given the internal connection and the queue that incoming objects
     *                 are sent to.
     */
    AsyncConnection( SOCKET sock, 
                     const Callback setup = nullptr, 
                     const Callback teardown = nullptr ) : 
        AsyncConnection( sock, std::make_shared<Queue>(), setup, nullptr ) {}

    /**
     * Closes the connection.
     */
    ~AsyncConnection() {

        close( true );

    }

    /**
     * Starts the connection, allowing the internal worker threads to start sending and
     * receiving messages.
     */
    void start() {

        // Start workers
        sender.start();
        receiver.start();

    }

    /**
     * Queues the given object for sending.
     *
     * @param e The object to send.
     * @throws std::invalid_argument if the given object is null.
     */
    void send( const Tptr & e ) {

        if ( e == nullptr ) {
            throw std::invalid_argument( "Cannot send null pointers." );
        }
        outQueue->push( e );

    }

    /**
     * Retrieves the next available incoming object, blocking until one is available if
     * necessary.
     *
     * @param dest Where to store the retrieved object into.
     * @param timeout If not 0, the maximum amount of time to block for if
     *                no objects are available.
     * @return True if an object was successfully retrieved, false if a timeout
     *         occurred. Will never be false if timeout is 0.
     */
    bool receive( Tptr & dest, const unsigned long timeout = 0 ) {

        return inQueue->pop( dest, timeout );

    }

    /**
     * Retrieves the next available incoming object, if there is one. If there are no
     * available objects, returns immediately.
     *
     * @param dest Where to store the retrieved object into.
     * @return True if an object was successfully retrieved, false if there are
     *         no objects available.
     */
    bool tryReceive( Tptr & dest ) {

        return inQueue->tryPop( dest );

    }

    /**
     * Retrieves all currently available incoming objects.
     *
     * @param dest Where to place the objects into.
     *             Elements are placed so that the oldest element is at
     *             the front of the queue.
     */
    void receiveAll( std::deque<Tptr> & dest ) {

        inQueue->popAll( dest );

    }

    /**
     * Retrieves the unterlying Connection object being used by
     * this instance for communication.
     *
     * @return The underlying Connection.
     */
    const std::shared_ptr<Connection> & connection() const {

        return conn;

    }

    /**
     * Closes the connection, stopping worker threads and releasing the underlying
     * connection.
     *
     * @param wait If true, blocks until the closing process is completed.
     */
    void close( bool wait = false ) {

        {
            std::lock_guard<std::mutex> lck( stop_mtx );
            if ( stopper == nullptr ) {
                stopper = std::make_shared<std::thread>( [this]() -> void {

                    // Send stop signal
                    outQueue->push( nullptr );

                    // Wait for workers to stop
                    sender.waitDone();
                    receiver.waitDone();

                    // Execute teardown callback
                    if ( teardown != nullptr ) {
                        teardown( conn, inQueue );
                    }

                    // Close connection
                    conn = nullptr; 

                } );
            }
        }

        if ( wait ) {
            std::lock_guard<std::mutex> lck( join_mtx );
            if ( stopper->joinable() ) {
                stopper->join();
            }
        }

    }

    /* The queue that incoming objects are stored in */
    const Qptr inQueue;

    protected:
    /**
     * Encodes an object into a message.
     * The encoding may NOT be an empty string.
     *
     * @param src The object to encode.
     * @param message Where to store the encoded message.
     */
    virtual void encode( const Tptr & src, std::string & message ) = 0;

    /**
     * Decodes an object from a message.
     *
     * @param message The message to decode.
     * @param dest Where to store the decoded object.
     */
    virtual void decode( const std::string & message, Tptr & dest ) = 0;

    /* The queue that outgoing objects are stored in */
    const Qptr outQueue;

    /* Worker that sends objects to the network */
    ThreadWorker sender;
    /* Worker that receives objects from the network */
    ThreadWorker receiver;

    /* Callback to execute during closing */
    const Callback teardown;

    private:
    /**
     * Action that the sender worker executes.
     *
     * @return False if the worker should stop, true otherwise.
     */
    bool senderLoop() {

        LOGGER->trace( "Outbound queue has {} items.", outQueue->size() );
        
        // Obtain next object to send
        Tptr src;
        outQueue->pop( src );
        if ( src == nullptr ) { // Received sentinel
            LOGGER->info( "Signaling connection end to peer." );
            conn->send( "" ); // Send sentinel
            return false; // Stop
        }

        // Encode object
        std::string message;
        encode( src, message );
        if ( message.empty() ) { // Empty string reserved as a sentinel
            LOGGER->error( "Object encoding was an empty string." );
            return true; // Discard
        }

        // Attach counter
        static unsigned int counter = 0;
        message = std::to_string( counter++ ) + ":" + message;

        // Send
        LOGGER->trace( "Sending encoded message '{}'", message );
        conn->send( message );

        return true;

    }

    /**
     * Action that the receiver worker executes.
     *
     * @return False if the worker should stop, true otherwise.
     */
    bool receiverLoop() {

        LOGGER->trace( "Inbound queue has {} items.", inQueue->size() );

        // Obtain next message
        std::string message = conn->receive();
        if ( message.empty() ) { // Received sentinel
            LOGGER->info( "Peer signaled connection end." );
            close( false ); // Signal stopping
            return false; // Stop
        }

        // Remove counter
        size_t pos = message.find( ":" );
        if ( pos != std::string::npos ) {
            message = message.substr( pos + 1 );
        } else {
            LOGGER->error( "Message missing counter: '{}'", message );
        }

        // Decode object
        LOGGER->trace( "Received encoded message '{}'", message );
        Tptr dest;
        decode( message, dest );

        if ( dest == nullptr ) {
            LOGGER->error( "Message could not get decoded: '{}'", message );
        } else {
            // Queue received object
            inQueue->push( dest );
        }
        return true;

    }

    std::shared_ptr<Connection> conn;
    std::mutex stop_mtx;
    std::mutex join_mtx;
    std::shared_ptr<std::thread> stopper;

    inline static const auto LOGGER = getLogger( "AsyncConnection" );

};

#endif