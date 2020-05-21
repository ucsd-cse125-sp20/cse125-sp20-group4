#include <string>
#include <functional>
#include "wavehandler.h"
#include "logger.h"

WaveHandler::WaveHandler(GameState& state) : waveNum(0), waveActive(false) {
    startWave = [this, &state]() {
        state.createTimer(ID_WAVE_TIMER, DEFAULT_WAVE_TIME, this->endWave);
        auto log = getLogger("WaveHandler");
        this->waveNum++;
        this->waveActive = true;
        // TODO instantiate enemies
        // TODO register handler that checks when all enemis are dead
        log->info("Wave {} started", this->waveNum);
    };
    endWave = [this, &state]() {
        state.createTimer(ID_READY_TIMER, DEFAULT_READY_TIME, this->startWave);
        this->waveActive = false;
        // TODO clean up remaining existing enemies
    };
}

void WaveHandler::init() {
    endWave();
}