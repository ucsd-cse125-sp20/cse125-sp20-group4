#include "EventClasses/GameState/place.h"
#include "ObjectClasses/barricade.h"
#include "logger.h"
PlaceBarricadeEvent::PlaceBarricadeEvent(std::string id) : GameStateEvent(id) {}

void PlaceBarricadeEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("PlaceBarricade");
    // get object
    std::shared_ptr<Object> object = gamestate->getObject(this->getObjectId());
    // check if found
    if (object != nullptr) {
        glm::vec3 pos = glm::vec3(object->getPosition());
        pos = pos + object->getOrientation()*2.0f;
        // get item in player inventory
        // TODO replace player with correct class
        std::shared_ptr<Object> nObject = std::shared_ptr<Object>(new Barricade("ignore", pos.x, pos.y, pos.z));
        log->info("Trying to place {}", nObject->serialize());
        gamestate->createObject(nObject);
    }
}

std::string PlaceBarricadeEvent::serialize() const {
    return "PlaceBarricade:" + Event::serialize();
}