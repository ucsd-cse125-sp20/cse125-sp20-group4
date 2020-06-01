#pragma once
#include "object.h"
#include <glm/glm.hpp>


class MovingObject : public Object {
private:
    glm::vec3 velocity;
    glm::vec3 relativeVelocity;
    glm::mat4x4 toWorld, fromWorld;
    bool isCollidedX = false;
    bool isCollidedY = false;
    bool isCollidedZ = false;

public:
    MovingObject(const MovingObject& obj);
    MovingObject(std::string id);
    MovingObject(std::string id, float x, float y, float z);
    MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length);
    MovingObject(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float width, float height, float length, float velX, float velY, float velZ);

    void setVelocityX(float velX);
    void setVelocityY(float velY);
    void setVelocityZ(float velZ);
    void setVelocity(float velX, float velY, float velZ);
    void setVelocity(glm::vec3 velocity);
    float getVelocityX() const;
    float getVelocityY() const;
    float getVelocityZ() const;
    const glm::vec3 & getVelocity() const;
    float getNextPositionX() const;
    float getNextPositionY() const;
    float getNextPositionZ() const;
    glm::vec3 getNextPosition() const;

    float getNextPositionCollisionX();
    float getNextPositionCollisionY();
    float getNextPositionCollisionZ();
    glm::vec3 getNextPositionCollision();

    void setOrientation( const glm::vec3 & orientation );
    void setRelativeVelocity( const glm::vec3 & newVelocity );
    const glm::vec3 & getRelativeVelocity() const;


    //utility for collision
    bool containsNext(const glm::vec3 pt) const;
    bool contains(const glm::vec3 pt) const;
    bool collides(const Object & obj) const;
    bool collides(const MovingObject & obj) const;
    bool collidesNext(const Object & obj) const;
    bool collidesNext(const MovingObject & obj) const;
    void handleXCollision(const Object & obj);
    void handleXCollision(const MovingObject & obj);
    void handleYCollision(const Object & obj);
    void handleYCollision(const MovingObject & obj);
    void handleZCollision(const Object & obj);
    void handleZCollision(const MovingObject & obj);

    virtual std::shared_ptr<Object> clone() const override;

    //(de)serialize functions
    virtual std::string serialize() const override;
};