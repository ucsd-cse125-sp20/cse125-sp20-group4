#include "ObjectClasses/enemy.h"
#include "logger.h"

#include <cmath>

Enemy::Enemy(const Enemy& enemy) : Enemy(enemy.getId(), enemy.getPositionX(), enemy.getPositionY(), enemy.getPositionZ(), enemy.getOrientationX(), enemy.getOrientationY(), enemy.getOrientationZ(), enemy.getVelocityX(), enemy.getVelocityY(), enemy.getVelocityZ()) {}
Enemy::Enemy(std::string id) : Enemy(id, 0, 0, 0) {}
Enemy::Enemy(std::string id, float x, float y, float z) : Enemy(id, x, y, z, 1.0f, 0.0f, 0.0f) {}
Enemy::Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ) : Enemy(id, x, y, z, orientationX, orientationY, orientationZ, 0.0f, 0.0f, 0.0f) {}
Enemy::Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, velX, velY, velZ) {
    auto log = getLogger("Enemy");
    log->trace("Creating Enemy with id {}, position ({}, {}, {}), orientation ({}, {}), velocity ({}, {}, {})", id, x, y, z, orientationX, orientationY, velX, velY, velZ);
}

// in game loop, if total displacement is less than some threshold,
// pop command and call this again
void Enemy::setVelocityFromCmd() {
    float dispX = this->getNextPositionX() - (float) cmdStack.top().x;
    float dispY = this->getNextPositionY() - (float) cmdStack.top().y;
    float dispTotal = sqrt(pow(dispX, 2) + pow(dispY, 2));
    setVelocityX(baseSpeed * dispX / dispTotal);
    setVelocityY(baseSpeed * dispY / dispTotal);
}

std::string Enemy::serialize() const {
    auto log = getLogger("Enemy");
    std::string res = "Enemy:" + MovingObject::serialize() + "," + std::to_string(static_cast<int>(weakness));
    log->trace("Serialized Enemy as {}", res);
    return res;
}