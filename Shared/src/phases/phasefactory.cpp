#include "phases/phasefactory.h"

std::shared_ptr<Phase> PhaseFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);

    // state
    pos = serial.find(",", last);
    int state = std::stoi(serial.substr(last, pos - last));
    last = pos + 1;

    // health
    pos = serial.find(",", last);
    int hp = std::stoi(serial.substr(last, pos - last));
    last = pos + 1;

    // count
    pos = serial.find(",", last);
    int cnt = std::stoi(serial.substr(last, pos - last));
    last = pos + 1;

    // wave
    pos = serial.find(",", last);
    int wave = std::stoi(serial.substr(last, pos - last));

    std::shared_ptr<Phase> phase = std::make_shared<Phase>(state, hp,cnt,wave);
    return phase;
}