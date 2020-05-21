#include "ObjectClasses/wall.h"
#include "logger.h"

Wall::Wall(std::string& id, float xPos, float yPos, float zPos) : Object(id, xPos, yPos, zPos) {
    setCanCollide(true);
}


std::string Wall::serialize() const {
    auto log = getLogger("Wall");
    std::string res = "Wall:" + Object::serialize();
    log->trace("Serialized Walls as {}", res);
    return res;

}