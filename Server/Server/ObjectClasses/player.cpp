#include "player.h"

Player::Player(int id) : MovingObject(id) {}

Player::Player(int id, int x, int y, int z) : MovingObject(id, x, y, z) {}
Player::Player(int id, int x, int y, int z, float hor, float ver) : MovingObject(id, x, y, z, hor, ver) {}
Player::Player(int id, int x, int y, int z, float hor, float ver, float velX, float velY, float velZ) : MovingObject(id, x, y, z, hor, ver, velX, velY, velZ) {}