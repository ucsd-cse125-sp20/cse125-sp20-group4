#ifndef CONNECTION_H
#define CONNECTION_H

#include <winsock2.h>

/**
 * Provides an interface for sending and receiving strings
 * over a socket.
 */
class Connection {

    public:
    /**
     * Creates a new instance that uses the given socket for connection.
     *
     * @param sock The socket to communicate through.
     */
    Connection( SOCKET sock );
    /**
     * Closes the connection.
     * This will also close the associated socket.
     */
    ~Connection();

    /**
     * Sends a message over the socket.
     * 
     * @param message The message to send.
     * @throws std::runtime_error if an error occured while sending.
     */
    void send( const std::string & message );
    /**
     * Receives a message from the socket.
     * The call will block until a full message is read from the socket.
     *
     * @return The received message.
     * @throws std::runtime_error if an error occured while receiving.
     */
    std::string receive();

    /**
     * Sanitizes a string for insertion in a packet.
     *
     * @param message The message to sanitize.
     * @return The sanitized message.
     */
    static std::string sanitize( const std::string & message );
    /**
     * Restores the original message from its sanitized version.
     * 
     * @param sanitized The sanitized message.
     * @return The original message.
     */
    static std::string restore( const std::string & sanitized );

    /**
     * Appends the given message to the buffer in a format ready to be sent over
     * the socket.
     *
     * @param message The message to insert.
     * @param buffer The buffer to insert into.
     */
    static void insertMessage( const std::string & message, std::string & buffer );
    /**
     * Retrieves from the given buffer the next message that was inserted using
     * insertMessage().
     *
     * After the call returns, any data that appears after the retrieved message is placed
     * back into the buffer for later reading. If there were no full messages found, but
     * a partial message was found at the end of the string, it is placed back in the buffer.
     * Any leading data that is not part of a message or is an incomplete message that precedes
     * a full message is discarded.
     *
     * @param buffer The buffer to extract a message from, and place leftover data into.
     * @param message Where to store the read message. If a full message isn't found, is
     *                set to an empty string.
     * @return True if a message was read, false if a full message was not found in the buffer.
     */
    static bool extractMessage( std::string & buffer, std::string & message );

    protected:
    /* The socket being used for communication */
    const SOCKET sock;

    private:
    /* Buffer used to store incoming data */
    std::string inboundBuffer;

};

#endif