#include "ObjectClasses/Factories/playerfactory.h"
std::shared_ptr<Object> PlayerFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id, posx, posy, posz, orx, ory, orz, velx, vely, velz
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    last = pos + 1;
    pos = serial.find(",", last);
    float posx = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float posy = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float posz = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float orx = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float ory = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float orz = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float velx = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float vely = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float velz = std::stof(serial.substr(last));

    std::shared_ptr<Player> player = std::make_shared<Player>(id, posx, posy, posz, orx, ory, orz, velx, vely, velz);
    return player;
}