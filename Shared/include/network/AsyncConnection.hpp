#ifndef ASYNC_CONNECTION_H
#define ASYNC_CONNECTION_H

#include "network/Connection.h"
#include "util/BlockingQueue.hpp"

template <class T>
class AsyncConnection {

    public:
    AsyncConnection( SOCKET sock, const std::shared_ptr<BlockingQueue<T>> & outQueue );

    void send( const T & e );
    T receive();

    const std::shared_ptr<Connection> & connection();

    protected:
    const std::shared_ptr<BlockingQueue<T>> inQueue;
    const std::shared_ptr<BlockingQueue<T>> outQueue;

    ThreadWorker receiver;
    ThreadWorker sender;

    private:
    std::shared_ptr<Connection> conn;

};

#endif