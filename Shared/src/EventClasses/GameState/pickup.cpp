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
    std::shared_ptr<Shelf> itemObject = std::dynamic_pointer_cast<Shelf>(gamestate->getObject(this->targetId));

    // check if found
    if (object != nullptr && itemObject != nullptr && object->getHeldItem()==nullptr) {
        if (glm::distance(object->getPosition(), itemObject->getPosition()) < 2.0f) {
            std::shared_ptr<Object> item = itemObject->getItem();
            if (std::dynamic_pointer_cast<Barricade>(item) != nullptr) {
                // check if player has enough dough
                if (object->getMoney() > 5) {
                    object->subtractMoney(5);
                    // put item into inventory
                    object->setHeldItem(itemObject->getItem());
                }
            } else {
                // put item into inventory
                object->setHeldItem(itemObject->getItem());
            }
            gamestate->setDirty(true);
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
