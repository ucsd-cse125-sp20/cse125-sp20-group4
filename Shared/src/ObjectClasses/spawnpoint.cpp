#include "..\..\include\ObjectClasses\spawnpoint.h"

SpawnPoint::SpawnPoint()
{
}

SpawnPoint::SpawnPoint(const SpawnPoint& obj)
{
}

SpawnPoint::SpawnPoint(const std::string& id, float xPos, float yPos, float zPos, float orx, float ory, float orz, float width, float height, float length)
{
}

const std::string& SpawnPoint::getTag()
{
    // TODO: insert return statement here
}

std::string SpawnPoint::serialize() const
{
    return std::string();
}

std::shared_ptr<Object> SpawnPoint::clone() const
{
    return std::shared_ptr<Object>();
}
