#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "gamestate.h"

class WaveHandler {

    public:
    using Clock = std::chrono::system_clock;
    enum class State { NO_CHANGE, PRE_WAVE, WAVE, NOT_RUNNING };
    typedef struct {

        unsigned int count;

    } EnemyData;

    WaveHandler();

    void loadWaveData();
    void start();

    State update( const GameState & gs );
    State getWaveInfo( unsigned int & waveNum, Clock::time_point & startTime, std::vector<EnemyData> & waveEnemies );

    private:
    bool running;
    unsigned int waveNum;
    bool waveActive;
    Clock::time_point startTime;
    std::vector<std::vector<EnemyData>> waveEnemies;

};