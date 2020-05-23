#include <stdexcept>

#include "logger.h"
#include "util/ThreadWorker.h"

const auto LOGGER = getLogger( "ThreadWorker" );

ThreadWorker::ThreadWorker( const std::function<bool()> act ) : ThreadWorker( act, false ) {}

ThreadWorker::ThreadWorker( const std::function<bool()> act, bool allowEmptyAct ) : ready( false ), running( true ), act( act ), worker( [this]() -> void { this->run(); } ) {

    if ( !allowEmptyAct && act == nullptr ) {
        throw std::invalid_argument( "Action function cannot be empty." );
    }

}

void ThreadWorker::run() {

    // Wait for ready signal
    waitReady();

    LOGGER->info( "Worker starting." );

    try {
        while ( running && action() ); // Repeat until stop signal
    } catch ( std::exception & e ) {
        LOGGER->error( "Unhandled exception in worker: {}", e.what() );
    }

    LOGGER->info( "Worker stopped." );

}

bool ThreadWorker::action() {

    return act(); // Run registered function

}

void ThreadWorker::start() {

    // Signal start
    std::unique_lock<std::mutex> lck;
    ready = true;
    readyCond.notify_all();

}

void ThreadWorker::stop() {

    // Signal stop
    running = false;

}

void ThreadWorker::waitReady() {

    // Wait until ready signal set
    std::unique_lock<std::mutex> lck;
    while ( !ready ) readyCond.wait( lck );

}

void ThreadWorker::waitDone() {

    worker.join();

}

