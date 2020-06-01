#include "ObjectClasses/Factories/enemyfactory.h"

std::shared_ptr<Object> EnemyFactory::create(std::string serial) { //TODO:Make more robust to serialization errors
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
    float velz = std::stof(serial.substr(last, pos - last));

    last = pos + 1;
    pos = serial.find(",", last);
    int weakness = std::stoi(serial.substr(last, pos - last));

    std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(id, posx, posy, posz, orx, ory, orz, velx, vely, velz);

    switch (weakness)
    {
    case ItemType::RED:
        enemy->weakness = ItemType::RED;
        break;
    case ItemType::GREEN:
        enemy->weakness = ItemType::GREEN;
        break;
    case ItemType::BLUE:
        enemy->weakness = ItemType::BLUE;
        break;
    }
    
    return enemy;
}

std::shared_ptr<Object> EnemyFactory::create() {
    return std::make_shared<Enemy>("ignore");
}
