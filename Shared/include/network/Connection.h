#ifndef CONNECTION_H
#define CONNECTION_H

#include <winsock2.h>

class Connection {

    public:
    Connection( SOCKET sock );
    ~Connection();

    void send( const std::string & message );
    std::string receive();

    static std::string sanitize( const std::string & message );
    static std::string restore( const std::string & sanitized );

    static bool extractMessage( std::string & in, std::string & out );
    static void insertMessage( const std::string & in, std::string & out );

    protected:
    const SOCKET sock;

    private:
    std::string inboundBuffer;

};

#endif