// Server.cpp : Defines the entry point for the application.
//

#undef UNICODE

#include <atomic>
#include <chrono>
#include <deque>
#include <random>
#include <stdexcept>
#include <thread>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include <glm/vec3.hpp>

#include <EventClasses/event.h>
#include <EventClasses/UpdateEvent.h>
#include <EventClasses/Server/DeleteEvent.h>

#include <gamestate.h>
#include <logger.h>

#include "Clients.h"
#include "statehandler.h"
#include "wavehandler.h"
#include "deserializer.h"
#include "maploader.h"
#include "inih/INIReader.h"
#include "phases/updatephaseevent.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define LOGFILE_NAME "log/server.log"
#define LOGLEVEL spdlog::level::info

#define MAX_CLIENTS 5
#define SERVER_TICK 30
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8080"
#define DEFAULT_CONFIG_FILE "Config/config.ini"

#define SPAWNS_PER_TICK 1
#define SPAWN_DELAY 30

static const std::chrono::duration TICK_PERIOD = std::chrono::milliseconds( SERVER_TICK );

static std::atomic<bool> running = true;

std::unordered_map<std::string, std::string> config_map;


// handles each client socket
// data passed as pointer to GameThreadQueues struct
void handleGame( const std::shared_ptr<Clients> & clients ) {

    auto log = getLogger( "Server" );

    //getLogger("AsyncConnection")->set_level(spdlog::level::trace);
    std::default_random_engine rng;

    // ************** SETUP GAME STATE ****************
    GameState gameState;
    MapLoader::LoadMap("Maps/map2.txt", &gameState);
    gameState.initialize();

    WaveHandler waveHandler = WaveHandler();
    waveHandler.loadWaveData();

    GameStateHandler gameStateHandler;
    std::deque<std::shared_ptr<Enemy>> pendingSpawns;
    unsigned int spawnCooldown = 0;

    while ( running ) {

        std::chrono::time_point tickStart = std::chrono::steady_clock::now();

        // ************** GAME LOGIC START **************
        log->trace( "Start of gameplay loop" );

        gameState.resetDirty();
        
        if (spawnCooldown == 0) {
            for (unsigned int i = 0; i < SPAWNS_PER_TICK && !pendingSpawns.empty(); i++) {

                std::shared_ptr<Enemy> e = pendingSpawns.front();
                pendingSpawns.pop_front();
                log->info("Spawning enemy '{}'.", e->getId());
                gameState.createObject(e, e->getId());

            }
            log->info("{}", gameState.serialize());

            spawnCooldown = SPAWN_DELAY;
        }
        else {
            spawnCooldown--;
        }

        // TODO: client voting system?
        if (clients->getClientCount() > 0) {
            waveHandler.start();
        }

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
        if (gameState.phase->dirty) {
            // send 
            clients->broadcast(std::make_shared<UpdatePhaseEvent>(gameState.phase));
        }
        WaveHandler::State waveState = waveHandler.update( gameState );

        unsigned int waveNum;
        std::chrono::system_clock::time_point startTime;
        std::vector<WaveHandler::EnemyData> waveEnemies;
        waveHandler.getWaveInfo( waveNum, startTime, waveEnemies );

        switch ( waveState ) {

            case WaveHandler::State::PRE_WAVE:
                // Send time to clients
                break;

            case WaveHandler::State::WAVE:
            {
                std::vector<glm::vec3> spawns; // TODO: obtain spawns
                spawns.push_back( glm::vec3( 1.0f, 0.0f, 1.0f ) );
                spawns.push_back( glm::vec3( 5.0f, 5.0f, 5.0f ) );
                spawns.push_back( glm::vec3( 3.0f, 0.0f, 5.0f ) );
                if ( spawns.size() == 0 ) {
                    log->error( "No locations to spawn enemies were defined." );
                    break;
                }

                std::uniform_int_distribution<unsigned int> spawnIndices( 0, ( unsigned int ) spawns.size() - 1 );
                for ( auto it = waveEnemies.cbegin(); it != waveEnemies.cend(); it++ ) {
                    log->info( "Creating {} enemies of type '{}' on wave {}.", it->count, it->type, waveNum );
                    for ( unsigned int i = 0; i < it->count; i++ ) {
                        const glm::vec3 & spawn = spawns[spawnIndices( rng )];
                        const std::string id = "wave" + std::to_string( waveNum ) + "-enemy-" + it->type + "-" + std::to_string( i );
                        std::shared_ptr<Enemy> e = std::make_shared<Enemy>( id, spawn.x, spawn.y, spawn.z );
                        pendingSpawns.push_back( e );
                    }
                }
                spawnCooldown = 0;


                // Notify clients?
                break;
            }

            case WaveHandler::State::DONE:
                // Notify client that game is won
                break;

        }

        /*
        if ( spawnCooldown == 0 ) {
            for ( unsigned int i = 0; i < SPAWNS_PER_TICK && !pendingSpawns.empty(); i++ ) {

                std::shared_ptr<Enemy> e = pendingSpawns.front();
                pendingSpawns.pop_front();
                log->info( "Spawning enemy '{}'.", e->getId() );
                gameState.createObject( e, e->getId() );

            }
            log->info("{}", gameState.serialize());

            spawnCooldown = SPAWN_DELAY;
        } else {
            spawnCooldown--;
        }

        // TODO: client voting system?
        if ( clients->getClientCount() > 0 ) {
            waveHandler.start();
        }
        */
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
    
    //read config file
    INIReader config(DEFAULT_CONFIG_FILE);
    if (config.ParseError() < 0) {
        spdlog::critical("Error reading config file");
        return EXIT_FAILURE;
    }
    WSADATA wsaData;
    int status;
    std::string port = DEFAULT_PORT;
    std::string set_port = config.Get("server", "port", "");
    if (set_port == "") {
        spdlog::warn("Port not found in config file, using default");
    }
    else {
        port = set_port;
    }
    spdlog::info("Starting server on port: {}", port);


    // Initialize Winsock
    if ( ( status = WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) ) != 0 ) {
        spdlog::critical( "WSAStartup failed with error: {0:d}", status );
        return EXIT_FAILURE;
    }

    // Start client handler
    std::shared_ptr<Clients> clients = std::make_shared<Clients>( port );

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
