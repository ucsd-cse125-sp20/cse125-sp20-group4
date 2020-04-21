#include "player.h"
#include "../logger.h"

Player::Player(int id) : Player(id, 0, 0, 0) {}
Player::Player(int id, int x, int y, int z) : Player(id, x, y, z, 0.0f, 0.0f) {}
Player::Player(int id, int x, int y, int z, float orientationX, float orientationY) : Player(id, x, y, z, orientationX, orientationY, 0.0f, 0.0f, 0.0f) {}
Player::Player(int id, int x, int y, int z, float orientationX, float orientationY, float velX, float velY, float velZ) : MovingObject(id, x, y, z, orientationX, orientationY, velX, velY, velZ) {
    auto log = logger();
    log->trace("Creating Player with id {}, position ({}, {}, {}), orientation ({}, {}), velocity ({}, {}, {})", id, x, y, z, orientationX, orientationY, velX, velY, velZ);
}
std::string Player::serialize() {
    auto log = logger();
    std::string res = "P:" + MovingObject::serialize();
    log->trace("Serialized Player as {}", res);
}