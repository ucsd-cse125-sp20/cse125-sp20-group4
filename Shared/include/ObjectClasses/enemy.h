#pragma once
#include "movingobject.h"
#include "MapRep.h"
#include "ObjectClasses/Useable/useable.h"
#include <list>

class Enemy : public MovingObject {
private:
    float baseSpeed = 1.0;
    std::list<glm::vec3> pathList;

public:
    Enemy(const Enemy& player);
    Enemy(std::string id);
    Enemy(std::string id, float x, float y, float z);
    Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ);
    Enemy(std::string id, float x, float y, float z, float orientationX, float orientationY, float orientationZ, float velX, float velY, float velZ);

    const std::string TAG = "Enemy";
    const std::string& getTag();

    void setPathList(std::list<glm::vec3>);
    // sets the velocity from top of list
    void setVelocityFromCmd();

    //(de)serialize functions
    virtual std::string serialize() const override;
    virtual std::shared_ptr<Object> clone() const;

    virtual void handleXCollision(const Object& obj);
    virtual void handleXCollision(const MovingObject& obj);
    virtual void handleYCollision(const Object& obj);
    virtual void handleYCollision(const MovingObject& obj);
    virtual void handleZCollision(const Object& obj);
    virtual void handleZCollision(const MovingObject& obj);

    static Enemy& deserialize(std::string serial);
    ItemType weakness;
};