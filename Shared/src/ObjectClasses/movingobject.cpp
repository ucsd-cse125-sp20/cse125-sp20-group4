#include <glm/gtc/matrix_transform.hpp>

#include "ObjectClasses/movingobject.h"
#include "logger.h"

#include "glm/gtc/epsilon.hpp"
#define EPSILON 0.0005f

MovingObject::MovingObject(const MovingObject& obj) : MovingObject(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(), obj.getOrientationX(), obj.getOrientationY(), obj.getOrientationZ(), obj.getWidth(), obj.getHeight(), obj.getLength(), obj.getVelocityX(), obj.getVelocityY(), obj.getVelocityZ()) {}

MovingObject::MovingObject(std::string id) : MovingObject(id, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z) : MovingObject(id, x, y, z, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length) : MovingObject(id, x, y, z, orientationX, orientationY, orientationZ, width, length, height, 0.0f, 0.0f, 0.0f) {}

MovingObject::MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length, float velX, float velY, float velZ) : Object(id, x, y, z, orientationX, orientationY, orientationZ, width, height, length,true) {
    auto log = getLogger("MovingObject");
    this->fromWorld= glm::mat4(1.0);
    this->toWorld = glm::mat4(1.0);
    setVelocity(velX, velY, velZ);
    this->relativeVelocity = fromWorld * glm::vec4(velocity, 1.0f);
    log->trace("Creating MovingObject with id {}, position ({}, {}, {}), width {}, height {}, length {},  orientation ({}, {}, {}), velocity ({}, {}, {}), relative velocity ({}, {}, {})", id, x, y, z, width, height, length, orientationX, orientationY, orientationZ, velX, velY, velZ, this->relativeVelocity.x, this->relativeVelocity.y, this->relativeVelocity.z);
}

void MovingObject::setVelocityX(float velX) {
    setVelocity( velX, getVelocity().y, getVelocity().z );
}

void MovingObject::setVelocityY(float velY) {
    setVelocity( getVelocity().x, velY, getVelocity().z );
}

void MovingObject::setVelocityZ(float velZ) {
    setVelocity( getVelocity().x, getVelocity().y, velZ );
}

void MovingObject::setVelocity(float velX, float velY, float velZ) {
    setVelocity( glm::vec3( velX, velY, velZ ) );
}
void MovingObject::setVelocity(glm::vec3 vel) {
    getLogger( "MovingObject" )->trace( "Setting velocity of {} to ({},{},{})", this->getId(), vel.x, vel.y, vel.z );
    this->velocity = vel;
}

float MovingObject::getVelocityX() const {
    return this->getVelocity().x;
}

float MovingObject::getVelocityY() const {
    return this->getVelocity().y;
}

float MovingObject::getVelocityZ() const {
    return this->getVelocity().z;
}

const glm::vec3 & MovingObject::getVelocity() const {
    return this->velocity;
}

void MovingObject::setOrientation( const glm::vec3 & newOrientation ) {

    Object::setOrientation( newOrientation );

    static const glm::vec3 FOWARD( 0.0f, 0.0f, 1.0f ); // Foward direction
    static const glm::mat4x4 I( 1.0f );

    const glm::vec3 horizontalOrientation = glm::normalize( glm::vec3( newOrientation.x, 0.0f, newOrientation.z ) );
    const float angle = glm::acos( glm::dot( FOWARD, horizontalOrientation ) );
    const glm::vec3 axis = glm::normalize( glm::cross( FOWARD, horizontalOrientation ) );

    toWorld = glm::rotate( I, angle, axis );
    fromWorld = glm::inverse( toWorld );
    getLogger("MovingObject")->trace("Setting orientation of {} with relative vel ({},{},{})", this->serialize(), relativeVelocity.x, relativeVelocity.y, relativeVelocity.z);

    if (!glm::all(epsilonEqual(relativeVelocity, glm::vec3(0, 0, 0), EPSILON))) {
        setVelocity(this->toWorld * glm::vec4(this->relativeVelocity, 1.0f));
    }
    this->dirty = true;
}

void MovingObject::setRelativeVelocity( const glm::vec3 & newVelocity ) {

    getLogger( "MovingObject" )->trace( "Setting relative velocity of {} to ({},{},{})", this->getId(), newVelocity.x, newVelocity.y, newVelocity.z );
    relativeVelocity = newVelocity;
    if (glm::all(epsilonEqual(newVelocity, glm::vec3(0, 0, 0), EPSILON))) {
        velocity = glm::vec3(0, 0, 0);
    } else {
        velocity = toWorld * glm::vec4(glm::normalize(newVelocity), 1.0f);
    }
    getLogger("MovingObject")->trace("Setting velocity of {} to ({},{},{})", this->getId(), velocity.x, velocity.y, velocity.z);
    this->dirty = true;
}

const glm::vec3 & MovingObject::getRelativeVelocity() const {

    return relativeVelocity;

}

float MovingObject::getNextPositionX() const {
    return getPositionX() + getVelocityX()/50;
}

float MovingObject::getNextPositionY() const {
    return getPositionY() + getVelocityY() / 50;
}
float MovingObject::getNextPositionZ() const {
    return getPositionZ() + getVelocityZ() / 50;
}

float MovingObject::getNextPositionCollisionX(){
    if(isCollidedX){
        isCollidedX = false; 
        return getPositionX(); 
	}
    return getPositionX() + getVelocityX()/50;
}

float MovingObject::getNextPositionCollisionY(){
    if(isCollidedY){
        isCollidedY = false; 
        return getPositionY(); 
	}
    return getPositionY() + getVelocityY() / 50;
}
float MovingObject::getNextPositionCollisionZ(){
    if(isCollidedZ){
        isCollidedZ = false; 
        return getPositionZ(); 
	}
    return getPositionZ() + getVelocityZ() / 50;
}



glm::vec3 MovingObject::getNextPositionCollision(){
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

bool MovingObject::collides(const Object & obj) const {
    return collidesNext(obj);
}

bool MovingObject::collides(const MovingObject & obj) const {
    return collidesNext(obj);
}

bool MovingObject::collidesNext(const Object & obj) const {
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
    log->trace("Collision not detected");

    return false;

}

bool MovingObject::collidesNext(const MovingObject & obj) const {
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
    log->trace("Collision not detected");
    
    return false;

}

void MovingObject::handleXCollision(const Object & obj) {
    auto log = getLogger("MovingObject");

    if (obj.getPositionX() - (obj.getWidth() / 2) < this->getNextPositionX() + (this->getWidth() / 2) && obj.getPositionX() + (obj.getWidth() / 2) > this->getNextPositionX() - (this->getWidth() / 2)) {
        isCollidedX = true;
        log->trace("Collided X {} with {}", this->serialize(), obj.serialize());

    }

}

void MovingObject::handleXCollision(const MovingObject & obj) {
    auto log = getLogger("MovingObject");

    if (obj.getNextPositionX() - (obj.getWidth() / 2) < this->getNextPositionX() + (this->getWidth() / 2) && obj.getNextPositionX() + (obj.getWidth() / 2) > this->getNextPositionX() - (this->getWidth() / 2)) {
        isCollidedX = true;
        log->trace("Collided X {} with {}", this->serialize(), obj.serialize());

    }

}

void MovingObject::handleYCollision(const Object & obj) {
    auto log = getLogger("MovingObject");
    if (obj.getPositionY() - (obj.getHeight() / 2) < this->getNextPositionY() + (this->getHeight() / 2) && obj.getPositionY() + (obj.getHeight() / 2) > this->getNextPositionY() - (this->getHeight() / 2)) {
        isCollidedY = true;
        log->trace("Collided Y {} with {}", this->serialize(), obj.serialize());

    }
}

void MovingObject::handleYCollision(const MovingObject & obj) {
    auto log = getLogger("MovingObject");

    if (obj.getNextPositionY() - (obj.getHeight() / 2) < this->getNextPositionY() + (this->getHeight() / 2) && obj.getNextPositionY() + (obj.getHeight() / 2) > this->getNextPositionY() - (this->getHeight() / 2)) {
        isCollidedY = true;
        log->trace("Collided Y {} with {}", this->serialize(), obj.serialize());

    }

}

void MovingObject::handleZCollision(const Object & obj) {
    auto log = getLogger("MovingObject");

    if (obj.getPositionZ() - (obj.getLength() / 2) < this->getNextPositionZ() + (this->getLength() / 2) && obj.getPositionZ() + (obj.getLength() / 2) > this->getNextPositionZ() - (this->getLength() / 2)) {
        isCollidedZ = true;
        log->trace("Collided Z {} with {}", this->getNextPositionZ(), obj.getPositionZ());

    }

}

void MovingObject::handleZCollision(const MovingObject & obj) {
    auto log = getLogger("MovingObject");

    if (obj.getNextPositionZ() - (obj.getLength() / 2) < this->getNextPositionZ() + (this->getLength() / 2) && obj.getNextPositionZ() + (obj.getLength() / 2) > this->getNextPositionZ() - (this->getLength() / 2)) {
        isCollidedZ = true;
        log->trace("Collided Z {} with {}", this->getNextPositionZ(), obj.getNextPositionZ());

    }

}

std::shared_ptr<Object> MovingObject::clone() const {
    return std::make_shared<MovingObject>(*this);
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