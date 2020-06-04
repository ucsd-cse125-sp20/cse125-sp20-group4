#include "phases/phase.h"
Phase::Phase() : state(START_STATE), health(100), count(0), wave(0){}
Phase::Phase(int st, int hp, int cnt, int wave) : state(st), health(hp), count(cnt), wave(wave){}
std::string Phase::serialize() const {
    return  "Phase:" + std::to_string(state) + "," + std::to_string(health) + "," + std::to_string(count) + "," + std::to_string(wave) + ",";
}
void Phase::update(Phase & p) {
    this->count = p.count;
    this->health = p.health;
    this->state = p.state;
    this->wave = p.wave;
}