#include "ObjectClasses/enemy.h"
#include "logger.h"
#include "ObjectClasses/barricade.h"
#include <cmath>

#define MIN_DIST 0.05f

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
        float dispX = pathList.front().x - this->getPositionX();
        float dispY = pathList.front().y - this->getPositionY();
        float dispZ = pathList.front().z - this->getPositionZ();
        float dispTotal = sqrt(pow(dispX, 2) + pow(dispY, 2) + pow(dispZ, 2));
        // if displacement is basically 0, start moving to next command point
        if (dispTotal < MIN_DIST) {
            setPositionX(pathList.front().x);
            setPositionY(pathList.front().y);
            setPositionZ(pathList.front().z);
            pathList.pop_front();
        } else {
            setVelocityX(baseSpeed * dispX / dispTotal);
            setVelocityY(baseSpeed * dispY / dispTotal);
            setVelocityZ(baseSpeed * dispZ / dispTotal);
            break;
        }
    }
    if (pathList.empty()) {
        // no path, so no move
        setVelocityX(0);
        setVelocityY(0);
        setVelocityZ(0);
        reachedTarget = true;
    }
}


void Enemy::handleXCollision(const Object& obj) {
    MovingObject::handleXCollision(obj);
    try {
        const Barricade& barricade = dynamic_cast<const Barricade&>(obj);
        barricade.degradeBarricade();
    }
    catch (std::bad_cast e){
        //not a barricade
    }
}
    

void Enemy::handleXCollision(const MovingObject& obj) {
    MovingObject::handleXCollision(obj);
}

void Enemy::handleYCollision(const Object& obj) {
    MovingObject::handleYCollision(obj);
    try {
        const Barricade& barricade = dynamic_cast<const Barricade&>(obj);
        barricade.degradeBarricade();
    }
    catch (std::bad_cast e) {
        //not a barricade
    }

}

void Enemy::handleYCollision(const MovingObject& obj) {
    MovingObject::handleYCollision(obj);
}

void Enemy::handleZCollision(const Object& obj) {
    MovingObject::handleZCollision(obj);
    try {
        const Barricade& barricade = dynamic_cast<const Barricade&>(obj);
        barricade.degradeBarricade();
    }
    catch (std::bad_cast e) {
        //not a barricade
    }
}

void Enemy::handleZCollision(const MovingObject& obj) {
    MovingObject::handleZCollision(obj);
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