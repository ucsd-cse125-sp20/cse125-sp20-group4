#include "EventClasses\GameState\pickup.h"
#include "logger.h"
PickUpEvent::PickUpEvent(std::string id, std::string targetId) : GameStateEvent(id) {
    this->targetId = targetId;
}

void PickUpEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("PickUpEvent");

    // get object
    std::shared_ptr<Player> object = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
    std::shared_ptr<Object> itemObject = gamestate->getObject(this->targetId);

    // check if found
    if (object != nullptr && itemObject != nullptr && object->getHeldItem()==nullptr) {
        if (glm::distance(object->getPosition(), itemObject->getPosition()) < 2.0f) {
            // put item into inventory
            object->setHeldItem(itemObject);
            // delete
            gamestate->deleteObject(itemObject->getId());
            log->info("Just picked up an item from {}",itemObject->serialize());
        }
    } else {
        log->info("Trying to picked up but no");
    }
}

std::string PickUpEvent::serialize() const
{
    return "PickUpEvent:" + Event::serialize()+","+this->targetId;
}
