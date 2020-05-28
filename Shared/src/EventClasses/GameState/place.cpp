#include "EventClasses/GameState/place.h"
#include "ObjectClasses/barricade.h"
#include "logger.h"
PlaceEvent::PlaceEvent(std::string id) : GameStateEvent(id) {}

void PlaceEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("PlaceEvent");
    // get object
    std::shared_ptr<Player> object = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
    // check if found
    if (object != nullptr && object->getHeldItem()!=nullptr) {
        // get item in player inventory
        std::shared_ptr<Object> item = object->getHeldItem();
        // TODO replace player with correct class
        glm::vec3 pos = glm::vec3(object->getPosition());
        pos = pos + object->getOrientation() * 2.0f;
        item->setPosition(pos.x,pos.y,pos.z);
        gamestate->createObject(item,item->getId());
        object->setHeldItem(nullptr);
        
    }
}

std::string PlaceEvent::serialize() const {
    return "PlaceEvent:" + Event::serialize();
}