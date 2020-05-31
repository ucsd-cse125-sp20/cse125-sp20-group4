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

    waveNum = 0;
    waveActive = false;

}

static std::string timeToStr( const WaveHandler::Clock::time_point & t ) {

    std::time_t time = WaveHandler::Clock::to_time_t( t );
    char buf[BUFSIZ];
    ctime_s( buf, BUFSIZ, &time );
    buf[BUFSIZ - 1] = '\0'; // Juuuuust in case
    return std::string( buf );

}

void WaveHandler::start() {

    LOGGER->info( "Starting waves." );
    waveNum = 1;
    waveActive = false;
    startTime = START_TIME( DEFAULT_READY_TIME );
    running = true;
    LOGGER->debug( "Wave will start at {}.", timeToStr( startTime ) );

}

WaveHandler::State WaveHandler::update( const GameState & gs ) {

    if ( !running ) {
        return State::NOT_RUNNING;
    }

    if ( waveActive ) {
        unsigned int remain = 0;
        for ( auto it = gs.getGameObjects().begin(); it != gs.getGameObjects().end(); it++ ) {
            if ( it->second->isEnemy() ) {
                remain++;
            }
        }
        LOGGER->trace( "Wave {} has {} enemies remaining.", waveNum, remain );
        if ( remain == 0 ) {
            // Wave over, start waiting for next wave
            LOGGER->info( "Wave {} completed, waiting for next wave.", waveNum );
            waveNum++;
            waveActive = false;
            startTime = START_TIME( DEFAULT_READY_TIME );
            LOGGER->debug( "Wave will start at {}.", timeToStr( startTime ) );
            return State::PRE_WAVE;
        }
    } else {
        if ( Clock::now() > startTime ) {
            // Start wave
            LOGGER->info( "Wave {} starting." );
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

    wave = waveNum;
    start = startTime;
    enemies = waveEnemies[waveNum];
    return waveActive ? State::WAVE : State::PRE_WAVE;

}