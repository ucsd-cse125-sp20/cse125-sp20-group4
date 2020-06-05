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
#include "ObjectClasses/toiletpaper.h"
#include "ObjectClasses/spawnpoint.h"
#include "EventClasses/SoundEvent.h"
#include "SoundQueue.h"

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
    MapLoader::LoadMap("Maps/map3.txt", &gameState);
    gameState.initialize();
    std::vector<glm::vec3> targets;
    // get all toilet paper targets
    for (auto it = gameState.getGameObjects().begin(); it != gameState.getGameObjects().end(); it++) {
        ToiletPaper* target = dynamic_cast<ToiletPaper*>(it->second.get());
        // check if dynamic cast not NULL
        if (target) {
            targets.push_back(target->getPosition());
        }
    }

    std::vector<glm::vec3> spawns; // TODO: obtain spawns
                // get all toilet paper targets
    for (auto it = gameState.getGameObjects().begin(); it != gameState.getGameObjects().end(); it++) {
        SpawnPoint* spawn = dynamic_cast<SpawnPoint*>(it->second.get());
        // check if dynamic cast not NULL
        if (spawn) {
            spawns.push_back(spawn->getPosition());
        }
    }

    std::uniform_int_distribution<int> targetIndices(0, targets.size() - 1);

    WaveHandler waveHandler = WaveHandler();
    waveHandler.loadWaveData();

    GameStateHandler gameStateHandler;
    std::deque<std::shared_ptr<Enemy>> pendingSpawns;
    unsigned int spawnCooldown = 0;

    //getLogger("PickUpEvent")->set_level(spdlog::level::trace);
    //getLogger("PlaceEvent")->set_level(spdlog::level::trace);
    while ( running ) {

        std::chrono::time_point tickStart = std::chrono::steady_clock::now();

        // ************** GAME LOGIC START **************
        log->trace( "Start of gameplay loop" );

        gameState.resetDirty();

        std::deque<std::shared_ptr<Event>> events;
        clients->receiveAll(events);
        log->trace("{} events in the event queue.", events.size());

        // process all events
        gameStateHandler.getNextState(&gameState, &events);

        switch (gameState.phase->state) {
        case START_STATE:
        case READY_STATE:
        case END_STATE:
            if (gameState.phase->count >= 4) {
                if (gameState.phase->state == READY_STATE) {
                    gameState.phase->wave++;
                } else {
                    gameState.phase->wave = 1;
                    gameState.phase->health = 100;
                }
                gameState.phase->state = ROUND_STATE;
                gameState.phase->dirty = true;
                waveHandler.start();
                log->info("STARTING ROUND");
            }
            break;
        case ROUND_STATE:
            if (gameState.phase->health <= 0) {
                gameState.phase->state = END_STATE;
                gameState.phase->dirty = true;
                // TODO: remove all enemies
                log->info("Game Over");
                SoundQueue::push( std::make_shared<SoundEvent>( "event:/game_over" ) );
                break;
            }
            if (spawnCooldown == 0) {

                for (unsigned int i = 0; i < SPAWNS_PER_TICK && !pendingSpawns.empty(); i++) {
                    std::shared_ptr<Enemy> e = pendingSpawns.front();
                    pendingSpawns.pop_front();
                    log->debug("Spawning enemy '{}'.", e->getId());
                    gameState.createObject(e, e->getId());
                    e->setPathList(gameState.map->getPath(e->getPosition(), targets[targetIndices(rng)]));
                }

                spawnCooldown = SPAWN_DELAY;
            } else {
                spawnCooldown--;
            }
            WaveHandler::State waveState = waveHandler.update(gameState);

            unsigned int waveNum;
            std::chrono::system_clock::time_point startTime;
            std::vector<WaveHandler::EnemyData> waveEnemies;
            waveHandler.getWaveInfo(waveNum, startTime, waveEnemies);

            switch (waveState) {

            case WaveHandler::State::PRE_WAVE:
                // Send time to clients
                if (gameState.phase->state != READY_STATE) {
                    gameState.phase->count = 0;
                    gameState.unready();
                    gameState.phase->state = READY_STATE;
                    gameState.phase->dirty = true;                    
                } 
                if (gameState.phase->count >= 4) {
                    waveHandler.start();
                }
                break;

            case WaveHandler::State::WAVE:
            {
                if ( spawns.size() == 0 ) {
                    log->error( "No locations to spawn enemies were defined." );
                    break;
                }
                std::vector<string> waveSpawns;
                std::uniform_int_distribution<unsigned int> spawnIndices(0, (unsigned int)spawns.size() - 1);
                for (auto it = waveEnemies.cbegin(); it != waveEnemies.cend(); it++) {
                    int wCount = 0;
                    int i;
                    
                    log->info("Creating {} enemies of type '{}' on wave {}.", it->count, it->type, waveNum);
                    for (unsigned int i = 0; i < it->count; i++) {
                        waveSpawns.push_back(it->type);
                    }
                    
                    
                }
                std::shuffle(std::begin(waveSpawns), std::end(waveSpawns), std::default_random_engine());
                for (unsigned int i = 0; i < waveSpawns.size(); i++) {
                    const glm::vec3& spawn = spawns[spawnIndices(rng)];
                    const std::string id = "wave" + std::to_string(waveNum) + "-enemy-" + waveSpawns[i] + "-" + std::to_string(i);
                    log->debug("Creating a {} on wave {}.", waveSpawns[i], waveNum);
                    std::shared_ptr<Enemy> e = std::make_shared<Enemy>(id, spawn.x, spawn.y, spawn.z);
                    if (waveSpawns[i] == "red") {
                        e->weakness = ItemType::RED;
                    }
                    else if (waveSpawns[i] == "green") {
                        e->weakness = ItemType::GREEN;
                    }
                    else {
                        e->weakness = ItemType::BLUE;
                    }
                    log->debug("enemy weak to {} created.", e->weakness);
                    pendingSpawns.push_back(e);
                }
                
                spawnCooldown = 0;

                break;
            }
            case WaveHandler::State::DONE:
                if (gameState.phase->state != END_STATE) {
                    gameState.phase->state = END_STATE;
                    gameState.phase->count = 0;
                    gameState.phase->dirty = true;
                    gameState.unready();
                }
                // Notify client that game is won
                break;
            }
            break;
        }
        
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

        std::deque<std::shared_ptr<SoundEvent>> soundEvents;
        SoundQueue::popAll( soundEvents );
        while ( !soundEvents.empty() ) {

            std::shared_ptr<SoundEvent> e = soundEvents.front();
            soundEvents.pop_front();
            clients->broadcast( e );

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
