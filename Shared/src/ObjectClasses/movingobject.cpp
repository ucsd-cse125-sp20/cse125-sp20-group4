#include "ObjectClasses/movingobject.h"
#include "logger.h"

MovingObject::MovingObject(std::string id) : MovingObject(id, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z) : MovingObject(id, x, y, z, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, width, length, height, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length, float velX, float velY, float velZ) : Object(id, x, y, z, orientationX, orientationY, orientationZ, width, height, length) {
    auto log = getLogger("MovingObject");
    setVelocity(velX, velY, velZ);
    log->trace("Creating MovingObject with id {}, position ({}, {}, {}), width {}, height {}, length {},  orientation ({}, {}, {}), velocity ({}, {}, {})", id, x, y, z, width, height, length, orientationX, orientationY, orientationZ, velX, velY, velZ);
}

void MovingObject::setVelocityX(float velX) {
    auto log = getLogger("MovingObject");
    this->velocity.x = velX;
    log->trace("Setting velocityX of MovingObject with id {}, with value {}", this->getId(), velX);
}

void MovingObject::setVelocityY(float velY) {
    auto log = getLogger("MovingObject");
    this->velocity.y = velY;
    log->trace("Setting velocityY of MovingObject with id {}, with value {}", this->getId(), velY);
}

void MovingObject::setVelocityZ(float velZ) {
    auto log = getLogger("MovingObject");
    this->velocity.z = velZ;
    log->trace("Setting velocityZ of MovingObject with id {}, with value {}", this->getId(), velZ);
}

void MovingObject::setVelocity(float velX, float velY, float velZ) {
    setVelocityX(velX);
    setVelocityY(velY);
    setVelocityZ(velZ);
}
void MovingObject::setVelocity(glm::vec3 vel) {
    setVelocityX(vel.x);
    setVelocityY(vel.y);
    setVelocityZ(vel.z);
}

float MovingObject::getVelocityX() const {
    return this->velocity.x;
}

float MovingObject::getVelocityY() const {
    return this->velocity.y;
}

float MovingObject::getVelocityZ() const {
    return this->velocity.z;
}

glm::vec3 MovingObject::getVelocity() const {
    return this->velocity;
}


float MovingObject::getNextPositionX() const {
    return getPositionX() + getVelocityX()/20;
}

float MovingObject::getNextPositionY() const {
    return getPositionY() + getVelocityY() / 20;
}
float MovingObject::getNextPositionZ() const {
    return getPositionZ() + getVelocityZ() / 20;
}

glm::vec3 MovingObject::getNextPosition() const {
    return glm::vec3(getNextPositionX(), getNextPositionY(), getNextPositionZ());
}

bool MovingObject::containsNext(const glm::vec3 pt) const {
    if (pt.x < this->getNextPositionX() + (this->getWidth() / 2) && pt.x > this->getNextPositionX() - (this->getWidth() / 2)) {
        return true;
    }
    if (pt.y < this->getNextPositionY() + (this->getHeight() / 2) && pt.y > this->getNextPositionY() - (this->getHeight() / 2)) {
        return true;
    }
    if (pt.z < this->getNextPositionZ() + (this->getLength() / 2) && pt.z > this->getNextPositionZ() - (this->getLength() / 2)) {
        return true;
    }
    return false;

}

bool MovingObject::contains(const glm::vec3 pt) const {
    return this->containsNext(pt);
}

bool MovingObject::collides(const Object obj) const {
    return collidesNext(obj);
}

bool MovingObject::collides(const MovingObject obj) const {
    return collidesNext(obj);
}

bool MovingObject::collidesNext(const Object obj) const {
    auto log = getLogger("MovingObject");

    if (obj.canCollide()) {
        if ((obj.getPositionX() - (obj.getWidth() / 2) < this->getNextPositionX() + (this->getWidth() / 2)) && (obj.getPositionX() + (obj.getWidth() / 2) > this->getNextPositionX() - (this->getWidth() / 2))) {
            if (obj.getPositionY() - (obj.getHeight() / 2) < this->getNextPositionY() + (this->getHeight() / 2) && obj.getPositionY() + (obj.getHeight() / 2) > this->getNextPositionY() - (this->getHeight() / 2)) {
                if (obj.getPositionZ() - (obj.getLength() / 2) < this->getNextPositionZ() + (this->getLength() / 2) && obj.getPositionZ() + (obj.getLength() / 2) > this->getNextPositionZ() - (this->getLength() / 2)) {
                    log->info("Collision detected");
                    return true;
                }
            }
        }
    }
    log->info("Collision not detected");

    return false;

}

bool MovingObject::collidesNext(const MovingObject obj) const {
    auto log = getLogger("MovingObject");
    if (obj.canCollide()) {
        if (obj.getNextPositionX() - (obj.getWidth() / 2) < this->getNextPositionX() + (this->getWidth() / 2) && obj.getNextPositionX() + (obj.getWidth() / 2) > this->getNextPositionX() - (this->getWidth() / 2)) {
            if (obj.getNextPositionY() - (obj.getHeight() / 2) < this->getNextPositionY() + (this->getHeight() / 2) && obj.getNextPositionY() + (obj.getHeight() / 2) > this->getNextPositionY() - (this->getHeight() / 2)) {
                if (obj.getNextPositionZ() - (obj.getLength() / 2) < this->getNextPositionZ() + (this->getLength() / 2) && obj.getNextPositionZ() + (obj.getLength() / 2) > this->getNextPositionZ() - (this->getLength() / 2)) {
                    log->info("Collision detected");
                    return true;
                }
            }
        }
    }
    log->info("Collision not detected");
    
    return false;

}

void MovingObject::handleXCollision(const Object obj) {
    auto log = getLogger("MovingObject");

    if (obj.getPositionX() - (obj.getWidth() / 2) < this->getNextPositionX() + (this->getWidth() / 2) && obj.getPositionX() + (obj.getWidth() / 2) > this->getNextPositionX() - (this->getWidth() / 2)) {
        this->setVelocityX(0);
        log->trace("Collided X {} with {}", this->serialize(), obj.serialize());

    }

}

void MovingObject::handleXCollision(const MovingObject obj) {
    auto log = getLogger("MovingObject");

    if (obj.getNextPositionX() - (obj.getWidth() / 2) < this->getNextPositionX() + (this->getWidth() / 2) && obj.getNextPositionX() + (obj.getWidth() / 2) > this->getNextPositionX() - (this->getWidth() / 2)) {
        this->setVelocityX(0);
        log->trace("Collided X {} with {}", this->serialize(), obj.serialize());

    }

}

void MovingObject::handleYCollision(const Object obj) {
    auto log = getLogger("MovingObject");
    if (obj.getPositionY() - (obj.getHeight() / 2) < this->getNextPositionY() + (this->getHeight() / 2) && obj.getPositionY() + (obj.getHeight() / 2) > this->getNextPositionY() - (this->getHeight() / 2)) {
        this->setVelocityY(0);
        log->trace("Collided Y {} with {}", this->serialize(), obj.serialize());

    }
}

void MovingObject::handleYCollision(const MovingObject obj) {
    auto log = getLogger("MovingObject");

    if (obj.getNextPositionY() - (obj.getHeight() / 2) < this->getNextPositionY() + (this->getHeight() / 2) && obj.getNextPositionY() + (obj.getHeight() / 2) > this->getNextPositionY() - (this->getHeight() / 2)) {
        this->setVelocityY(0);
        log->trace("Collided Y {} with {}", this->serialize(), obj.serialize());

    }

}

void MovingObject::handleZCollision(const Object obj) {
    auto log = getLogger("MovingObject");

    if (obj.getPositionZ() - (obj.getLength() / 2) < this->getNextPositionZ() + (this->getLength() / 2) && obj.getPositionZ() + (obj.getLength() / 2) > this->getNextPositionZ() - (this->getLength() / 2)) {
        this->setVelocityZ(0);
        log->trace("Collided Z {} with {}", this->getNextPositionZ(), obj.getPositionZ());

    }

}

void MovingObject::handleZCollision(const MovingObject obj) {
    auto log = getLogger("MovingObject");

    if (obj.getNextPositionZ() - (obj.getLength() / 2) < this->getNextPositionZ() + (this->getLength() / 2) && obj.getNextPositionZ() + (obj.getLength() / 2) > this->getNextPositionZ() - (this->getLength() / 2)) {
        this->setVelocityZ(0);
        log->trace("Collided Z {} with {}", this->getNextPositionZ(), obj.getNextPositionZ());

    }

}



//(de)serialize functions
std::string MovingObject::serialize() const {
    auto log = getLogger("MovingObject");
    std::string res = Object::serialize() + ","
        + std::to_string(velocity.x) + ","
        + std::to_string(velocity.y) + ","
        + std::to_string(velocity.z);
    log->trace("Serialized MovingObject as {}", res);
    return res;
}