#include <string>
#include <functional>
#include "wavehandler.h"
#include "logger.h"
#include "objectclasses/enemy.h"

WaveHandler::WaveHandler(GameState& state) : waveNum(0), waveActive(false) {
    startWave = [this, &state]() {
        state.createTimer(ID_WAVE_TIMER, DEFAULT_WAVE_TIME, this->endWave);
        auto log = getLogger("WaveHandler");
        this->waveNum++;
        this->waveActive = true;
        std::shared_ptr<Enemy> enemy = std::shared_ptr<Enemy>(new Enemy("enemy", 0.0, 0.0, 5.0)); // TODO set type to enemy when can be rendered on client
        enemy->weakness = ItemType::RED;
        state.createObject(enemy, enemy->getId());
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