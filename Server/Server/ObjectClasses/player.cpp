#include "player.h"

Player::Player(int id) : MovingObject(id) {}
Player::Player(int id, int x, int y, int z) : MovingObject(id, x, y, z) {}
Player::Player(int id, int x, int y, int z, float orientationX, float orientationY) : MovingObject(id, x, y, z, orientationX, orientationY) {}
Player::Player(int id, int x, int y, int z, float orientationX, float orientationY, float velX, float velY, float velZ) : MovingObject(id, x, y, z, orientationX, orientationY, velX, velY, velZ) {}
std::string Player::serialize() {
    return "P:" + MovingObject::serialize();
}