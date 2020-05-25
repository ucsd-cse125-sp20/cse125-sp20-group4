#pragma once
#include <string>
#include "gamestate.h"

//#define DEFAULT_READY_TIME 30000.0
#define DEFAULT_READY_TIME 10000.0
#define DEFAULT_WAVE_TIME 120000.0
#define ID_READY_TIMER std::string("ready_timer")
#define ID_WAVE_TIMER std::string("wave_timer")

class WaveHandler {
private:
    int waveNum;
    bool waveActive;
    std::function<void()> startWave;
    std::function<void()> endWave;

public:
    WaveHandler(GameState&);
    void init();
};