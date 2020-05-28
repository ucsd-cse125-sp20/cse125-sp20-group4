#include "ObjectClasses/Factories/playerfactory.h"
#include "deserializer.h"

std::shared_ptr<Object> PlayerFactory::create(std::string serial) { //TODO:Make more robust to serialization errors
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
    float width = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float height = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float length = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float velx = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float vely = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    float velz = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    int health = std::stoi(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    int money = std::stoi(serial.substr(last, pos - last));

    last = pos + 1;

    pos = serial.find("/", last);

    //handle this differently
    Deserializer deserializer;
    std::string heldString = serial.substr(last, pos - last);
    std::shared_ptr<Object> heldObj = nullptr;
    if (heldString != "noItem") {
        heldObj = deserializer.deserializeObject(heldString);
    }

  

    std::shared_ptr<Player> player = std::make_shared<Player>(id, posx, posy, posz, orx, ory, orz, width, height, length, velx, vely, velz, health, money, heldObj);
    return player;
}

std::shared_ptr<Object> PlayerFactory::create() {
    return std::make_shared<Player>("ignore");
}
