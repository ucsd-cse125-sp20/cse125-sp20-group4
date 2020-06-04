#pragma once
#include "movingobject.h"
#include "MapRep.h"
#include "ObjectClasses/Useable/useable.h"
#include <list>

class Enemy : public MovingObject {
private:
    float baseSpeed = 5.0;
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


    static Enemy& deserialize(std::string serial);
    ItemType weakness;
};