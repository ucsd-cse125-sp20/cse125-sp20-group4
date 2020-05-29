// Server.cpp : Defines the entry point for the application.
//

#undef UNICODE

#include <atomic>
#include <chrono>
#include <deque>
#include <stdexcept>
#include <thread>
#include <unordered_map>

#include <EventClasses/event.h>
#include <EventClasses/UpdateEvent.h>
#include <EventClasses/Server/DeleteEvent.h>

#include <gamestate.h>
#include <logger.h>

#include "Clients.h"
#include "statehandler.h"
#include "wavehandler.h"
#include "deserializer.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define LOGFILE_NAME "log/server.log"
#define LOGLEVEL spdlog::level::warn

#define MAX_CLIENTS 5
#define SERVER_TICK 20
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"

static const std::chrono::duration TICK_PERIOD = std::chrono::milliseconds( SERVER_TICK );

static std::atomic<bool> running = true;

// handles each client socket
// data passed as pointer to GameThreadQueues struct
void handleGame( const std::shared_ptr<Clients> & clients ) {

    auto log = getLogger( "Server" );

    // ************** SETUP GAME STATE ****************
    GameState gameState;
    gameState.initialize("map.txt");
    WaveHandler waveHandler = WaveHandler(gameState); // TODO move this into gamestate
    waveHandler.init();
    GameStateHandler gameStateHandler;

    while ( running ) {

        std::chrono::time_point tickStart = std::chrono::steady_clock::now();

        // ************** GAME LOGIC START **************
        log->trace( "Start of gameplay loop" );

        gameState.resetDirty();
        
        std::deque<std::shared_ptr<Event>> events;
        clients->receiveAll( events );
        log->trace( "{} events in the event queue.", events.size() );

        // process all events
        gameStateHandler.getNextState( &gameState, &events );
        // TODO: check if we have hit the tick yet

        // send out new gameState if gamestate has changed
        if (gameState.isDirty()) {
            std::shared_ptr<UpdateEvent> update = std::make_shared<UpdateEvent>( std::unordered_map<std::string, std::shared_ptr<Object>>( gameState.getGameObjects().cbegin(), gameState.getGameObjects().cend() ) );
            clients->broadcast( update );
        }
        if (gameState.deletes) {
            std::shared_ptr<DeleteEvent> deletes = std::make_shared<DeleteEvent>(gameState.getDeletions());
            clients->broadcast(deletes);
        }
        
        // *************** GAME LOGIC END ***************
        std::chrono::time_point tickEnd = std::chrono::steady_clock::now();
        std::chrono::duration tickTime = std::chrono::duration_cast<std::chrono::milliseconds>( tickEnd - tickStart );
        log->trace( "Tick time: {0:d} ms", tickTime.count() );
        if ( tickTime < TICK_PERIOD ) {
            std::this_thread::sleep_for( TICK_PERIOD - tickTime );
        } else if ( tickTime > TICK_PERIOD ) {
            log->warn( "Tick took {0:d} ms", tickTime.count() );
        }
        tickStart = tickEnd;

    }

}

int main_inner( void ) {
  
    //std::shared_ptr<Client> c = std::make_shared<Client>( 0, nullptr, nullptr, nullptr );

    WSADATA wsaData;
    int status;

    // Initialize Winsock
    if ( ( status = WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) ) != 0 ) {
        spdlog::critical( "WSAStartup failed with error: {0:d}", status );
        return EXIT_FAILURE;
    }

    // Start client handler
    std::shared_ptr<Clients> clients = std::make_shared<Clients>( DEFAULT_PORT );

    // Create thread to handle game state loop
    std::thread gameThread( [clients]() -> void { handleGame( clients ); } );
    gameThread.join();

    // Release clients
    clients = nullptr;
    WSACleanup();
    return EXIT_SUCCESS;
}

int main( void ) {

    initLogging( LOGFILE_NAME, LOGLEVEL );
    spdlog::info( "Server starting up." );
    try {
        int statusCode = main_inner();
        spdlog::info( "Server shutting down." );
        shutdownLogging();
        return statusCode;
    } catch ( std::exception & e ) {
        spdlog::critical( "Unhandled exception: {}", e.what() );
        shutdownLogging();
        throw e; // Record and rethrow
    }

}
