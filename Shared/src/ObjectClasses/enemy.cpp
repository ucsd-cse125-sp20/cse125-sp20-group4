#include "ObjectClasses/enemy.h"
#include "logger.h"

#include <cmath>
const std::string& Enemy::getTag() {
    return TAG;
}
Enemy::Enemy(const Enemy& enemy) : Enemy(enemy.getId(), enemy.getPositionX(), enemy.getPositionY(), enemy.getPositionZ(), enemy.getOrientationX(), enemy.getOrientationY(), enemy.getOrientationZ(), enemy.getVelocityX(), enemy.getVelocityY(), enemy.getVelocityZ()) {}
Enemy::Enemy(std::string id) : Enemy(id, 0, 0, 0) {}
Enemy::Enemy(std::string id, float x, float y, float z) : Enemy(id, x, y, z, 1.0f, 0.0f, 0.0f) {}
Enemy::Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ) : Enemy(id, x, y, z, orientationX, orientationY, orientationZ, 0.0f, 0.0f, 0.0f) {}
Enemy::Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, velX, velY, velZ) {
    auto log = getLogger("Enemy");
    log->trace("Creating Enemy with id {}, position ({}, {}, {}), orientation ({}, {}), velocity ({}, {}, {})", id, x, y, z, orientationX, orientationY, velX, velY, velZ);
    setCanCollide(true);
}

void Enemy::setPathList(std::list<glm::vec3> inPathList) {
    pathList = inPathList;
}

// in game loop, if total displacement is less than some threshold,
// pop command and call this again
void Enemy::setVelocityFromCmd() {
    while (!pathList.empty()) {
        // calc displacement to next command point
        float dispX = this->getPositionX() - (float)pathList.front().x;
        float dispY = this->getPositionY() - (float)pathList.front().y;
        float dispZ = this->getPositionZ() - (float)pathList.front().z;
        float dispTotal = sqrt(pow(dispX, 2) + pow(dispY, 2) + pow(dispZ, 2));
        // if displacement is basically 0, move to next command point
        if (dispTotal < 0.0001) {
            pathList.pop_front();
        } else {
            setVelocityX(baseSpeed * dispX / dispTotal);
            setVelocityY(baseSpeed * dispY / dispTotal);
            setVelocityZ(baseSpeed * dispZ / dispTotal);
        }
    }
    if (pathList.empty()) {
        // no path, so no move
        setVelocityX(0);
        setVelocityY(0);
        setVelocityZ(0);
    }
}

std::shared_ptr<Object> Enemy::clone() const {
    return std::make_shared<Enemy>(*this);
}


std::string Enemy::serialize() const {
    auto log = getLogger("Enemy");
    std::string res = TAG+":" + MovingObject::serialize() + "," + std::to_string(static_cast<int>(weakness));
    log->trace("Serialized Enemy as {}", res);
    return res;
}