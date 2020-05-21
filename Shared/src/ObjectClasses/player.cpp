#include "ObjectClasses/player.h"
#include "logger.h"

Player::Player(const Player& player) : Player(player.getId(), player.getPositionX(), player.getPositionY(), player.getPositionZ(), player.getOrientationX(), player.getOrientationY(), player.getOrientationZ(), player.getVelocityX(), player.getVelocityY(), player.getVelocityZ()) {}
Player::Player(std::string id) : Player(id, 0, 0, 0) {}
Player::Player(std::string id, float x, float y, float z) : Player(id, x, y, z, 1.0f, 0.0f, 0.0f) {}
Player::Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ) : Player(id, x, y, z, orientationX, orientationY, orientationZ, 0.0f, 0.0f, 0.0f) {}
Player::Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float length, float height) : Player(id, x, y, z, orientationX, orientationY, orientationZ, width, length, height, 0.0f, 0.0f, 0.0f) {}

Player::Player(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length, float velX, float velY, float velZ) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, width, height, length, velX, velY, velZ) {
    auto log = getLogger("Player");
    log->trace("Creating Player with id {}, position ({}, {}, {}), width {}, height {}, length {}, orientation ({}, {}, {}), velocity ({}, {}, {})", id, x, y, z, width, height, length, orientationX, orientationY, orientationZ, velX, velY, velZ);
}

std::string Player::serialize() const {
    auto log = getLogger("Player");
    std::string res = "Player:" + MovingObject::serialize();
    log->trace("Serialized Player as {}", res);
    return res;
}