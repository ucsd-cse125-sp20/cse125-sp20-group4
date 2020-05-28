#include "EventClasses\GameState\pickup.h"
#include "logger.h"
PickUpEvent::PickUpEvent(std::string id, std::string targetId) : GameStateEvent(id), targetId(targetId) {}

void PickUpEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("PickUpEvent");
    // get object
    std::shared_ptr<Player> object = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
    std::shared_ptr<Object> itemObject = gamestate->getObject(this->targetId);
    // check if found
    if (object != nullptr && itemObject != nullptr) {
        if (glm::distance(object->getPosition(), itemObject->getPosition()) < 0.2f) {
            // put item into inventory
            //TODO get the type from the itemObject
            object->inventory = 1;
            log->info("Just picked up an item from {}",itemObject->serialize());
        }
    }
}

std::string PickUpEvent::serialize() const
{
    return "PickUpEvent:" + Event::serialize()+","+this->targetId;
}
