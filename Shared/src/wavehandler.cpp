#include <string>
#include <time.h>

#include "logger.h"
#include "objectclasses/enemy.h"
#include "wavehandler.h"

#define DEFAULT_READY_TIME 10 // In seconds

#define START_TIME( delay ) ( WaveHandler::Clock::now() + std::chrono::seconds( delay ) )

static const auto LOGGER = getLogger( "WaveHandler" );

WaveHandler::WaveHandler() : running( false ), waveNum( 0 ), waveActive( false ) {}

void WaveHandler::loadWaveData() {

    running = false;

    waveEnemies.clear();

    // TODO: set up waves
    std::vector<EnemyData> w1;
    EnemyData eRed;
    EnemyData eBlue;
    EnemyData eGreen;
    eRed.type = "red";
    eRed.count = 0; //30;
    eBlue.type = "blue";
    eBlue.count = 10;
    eGreen.type = "green";
    eGreen.count = 0;

    w1.push_back( eRed );
    w1.push_back(eBlue);
    w1.push_back(eGreen);


    waveEnemies.push_back( w1 );

    std::vector<EnemyData> w2;
    eRed.type = "red";
    eRed.count = 0; //30;
    eBlue.type = "blue";
    eBlue.count = 0;
    eGreen.type = "green";
    eGreen.count = 10;
    w2.push_back( eRed );
    w2.push_back(eBlue);
    w2.push_back(eGreen);
    waveEnemies.push_back( w2 );

    std::vector<EnemyData> w3;
    eRed.type = "red";
    eRed.count = 0; //30;
    eBlue.type = "blue";
    eBlue.count = 10;
    eGreen.type = "green";
    eGreen.count = 0;

    w3.push_back(eRed);
    w3.push_back(eBlue);
    w3.push_back(eGreen);
    waveEnemies.push_back(w3);

    waveNum = 0;
    waveActive = false;

}

static std::string timeToStr( const WaveHandler::Clock::time_point & t ) {

    std::time_t time = WaveHandler::Clock::to_time_t( t );
    char buf[BUFSIZ];
    ctime_s( buf, BUFSIZ, &time );
    buf[BUFSIZ - 1] = '\0'; // Juuuuust in case
    std::string s( buf );
    s.pop_back(); // Remove trailing newline
    return s;

}

void WaveHandler::start() {

    if ( !running ) {
        LOGGER->info( "Starting waves." );
        waveNum = 1;
        waveActive = false;
        startTime = START_TIME( DEFAULT_READY_TIME );
        running = true;
        LOGGER->debug( "Wave will start at {}.", timeToStr( startTime ) );
    }

}

WaveHandler::State WaveHandler::update( const GameState & gs ) {

    if ( !running ) {
        return State::NOT_RUNNING;
    }

    if ( waveNum > waveEnemies.size() ) {
        return State::NO_CHANGE;
    }

    if ( waveActive ) {
        unsigned int remain = 0;
        for ( auto it = gs.getGameObjects().begin(); it != gs.getGameObjects().end(); it++ ) {
            Enemy *enemy = dynamic_cast<Enemy*>(it->second.get());
            // check if dynamic cast not NULL
            if (enemy) {
                enemy->setVelocityFromCmd();
                remain++;
            }
        }
        LOGGER->trace( "Wave {} has {} enemies remaining.", waveNum, remain );
        if ( remain == 0 ) {
            // Wave over, start waiting for next wave
            LOGGER->info( "Wave {} completed, waiting for next wave.", waveNum );
            waveNum++;
            waveActive = false;
            if ( waveNum > waveEnemies.size() ) {
                LOGGER->info( "All waves done." );
                return State::DONE;
            } else {
                startTime = START_TIME( DEFAULT_READY_TIME );
                LOGGER->debug( "Wave will start at {}.", timeToStr( startTime ) );
                return State::PRE_WAVE;
            }
        }
    } else {
        if ( Clock::now() > startTime ) {
            // Start wave
            LOGGER->info( "Wave {} starting.", waveNum );
            waveActive = true;
            return State::WAVE;
        }
    }

    return State::NO_CHANGE;

}

WaveHandler::State WaveHandler::getWaveInfo( unsigned int & wave, Clock::time_point & start, std::vector<EnemyData> & enemies ) {

    if ( !running ) {
        return State::NOT_RUNNING;
    }

    if ( waveNum > waveEnemies.size() ) {
        return State::DONE;
    }

    wave = waveNum;
    start = startTime;
    enemies = waveEnemies[waveNum - 1];
    return waveActive ? State::WAVE : State::PRE_WAVE;

}