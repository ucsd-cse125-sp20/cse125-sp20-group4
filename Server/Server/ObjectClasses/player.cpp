#include "player.h"
#include "Shared/logger.h"

Player::Player(int id) : Player(id, 0, 0, 0) {}
Player::Player(int id, float x, float y, float z) : Player(id, x, y, z, 0.0f, 0.0f, 0,0f) {}
Player::Player(int id, float x, float y, float z, float orientationX, float orientationY, float orientationZ) : Player(id, x, y, z, orientationX, orientationY, orientationZ, 0.0f, 0.0f, 0.0f) {}
Player::Player(int id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, velX, velY, velZ) {
    auto log = logger();
    log->trace("Creating Player with id {}, position ({}, {}, {}), orientation ({}, {}), velocity ({}, {}, {})", id, x, y, z, orientationX, orientationY, velX, velY, velZ);
}
std::string Player::serialize() {
    auto log = logger();
    std::string res = "P:" + MovingObject::serialize();
    log->trace("Serialized Player as {}", res);
}