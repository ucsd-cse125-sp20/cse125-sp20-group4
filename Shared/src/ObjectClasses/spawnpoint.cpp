#include "ObjectClasses\spawnpoint.h"


SpawnPoint::SpawnPoint() : Object("ignore") {}

SpawnPoint::SpawnPoint(const SpawnPoint& obj) : SpawnPoint(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(), obj.getOrientationX(), obj.getOrientationY(), obj.getOrientationZ(), obj.getWidth(), obj.getHeight(), obj.getLength()) {}

SpawnPoint::SpawnPoint(const std::string& id, float xPos, float yPos, float zPos, float orx, float ory, float orz, float width, float height, float length) : Object(id, xPos, yPos, zPos, orx, ory, orz, width, height, length) {}

const std::string& SpawnPoint::getTag()
{
    return TAG;
}

std::string SpawnPoint::serialize() const
{
    return "SpawnPoint:" + Object::serialize();
}

std::shared_ptr<Object> SpawnPoint::clone() const
{
    return std::make_shared<SpawnPoint>(this->getId(), this->getPositionX(), this->getPositionY(), this->getPositionZ(), this->getOrientationX(), this->getOrientationY(), this->getOrientationZ(), this->getWidth(), this->getHeight(), this->getLength());
}