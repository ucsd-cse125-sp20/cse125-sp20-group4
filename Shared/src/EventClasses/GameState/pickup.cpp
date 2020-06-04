#include "EventClasses\GameState\pickup.h"
#include "logger.h"
PickUpEvent::PickUpEvent(std::string id, std::string targetId) : GameStateEvent(id) {
    this->targetId = targetId;
}

void PickUpEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("PickUpEvent");
    log->trace("Pick up event: {}", serialize());
    // get object
    std::shared_ptr<Player> object = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
    // calculate where player is looking
    glm::vec3 dir = glm::vec3(object->getOrientationX(), 0.0f, object->getOrientationZ());

    std::shared_ptr<Shelf> itemObject = std::dynamic_pointer_cast<Shelf>(gamestate->map->getObjectAtPos(object->getPosition() + dir*1.2f));
    std::shared_ptr<Barricade> barricade = std::dynamic_pointer_cast<Barricade>(gamestate->map->getObjectAtPos(object->getPosition() + dir * 1.5f));

    // check if found
    if (object != nullptr && itemObject != nullptr && object->getHeldItem()==nullptr) {
        log->trace("passed first check trying to get item from {}",itemObject->getId());
        if (glm::distance(object->getPosition(), itemObject->getPosition()) < 2.0f && itemObject->getFactory() != nullptr) {
            log->trace("passed second check");
            std::shared_ptr<Object> item = itemObject->getItem();
            if (item->getTag().compare("Barricade")==0) {
                // check if player has enough dough
                if (object->getMoney() > 5) {
                    object->subtractMoney(5);
                    // put item into inventory
                    object->setHeldItem(item);
                }
            } else {
                // put item into inventory
                object->setHeldItem(item);
            }
            gamestate->setDirty(true);
            log->debug("Just picked up an item {}",item->serialize());
        }
    } if (barricade != nullptr && object->getHeldItem() == nullptr) {
        log->trace("passed first check trying to get item from {}", barricade->getId());
        if (glm::distance(object->getPosition(), barricade->getPosition()) < 2.0f) {
            log->warn("passed second check");
            auto bar = std::make_shared<Barricade>();
            bar->setDurability(barricade->getDurability());
            bar->setUp(barricade->isUp());
            object->setHeldItem(bar);
            gamestate->deleteObject(barricade->getId());
        }
    } else {
        glm::vec3 tmp = object->getPosition() + dir * 1.5f;
        log->trace("Trying to picked up but no {}, {}, {}",tmp.x,tmp.y,tmp.z);
    }
}

std::string PickUpEvent::serialize() const
{
    return "PickUpEvent:" + Event::serialize()+","+this->targetId;
}
