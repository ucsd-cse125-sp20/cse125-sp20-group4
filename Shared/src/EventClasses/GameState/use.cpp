#include "EventClasses\GameState\use.h"
#include "ObjectClasses/Useable/useable.h"
#include "logger.h"
#include <glm/glm.hpp>
UseEvent::UseEvent(std::string id) : GameStateEvent(id) {}

const std::string UseEvent::TAG = "UseEvent";
void UseEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("UseEvent");
    // get player
    std::shared_ptr<Player> object = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
    // check if found
    if (object != nullptr && object->getHeldItem() != nullptr && object->getHeldItem()->getTag().compare("Barricade")!=0) {
        log->debug("Using item");
        // get item in player inventory
        std::shared_ptr<UseableObject> item = std::dynamic_pointer_cast<UseableObject>(object->getHeldItem());
        // TODO: determine how to use event
        ItemType itype = item->itemType;
        // loop over gamestate and remove all matching over add money for each removed
        std::deque<std::string> deletes;
        int count = 0;
        for (auto it = gamestate->getGameObjects().begin(); it != gamestate->getGameObjects().end();it++) {
            auto enemy = std::dynamic_pointer_cast<Enemy>(it->second);
            if ( enemy != nullptr && glm::distance(object->getPosition(),enemy->getPosition())<5.0f && enemy->weakness == itype) {
                deletes.push_back(enemy->getId());
                count++;
                object->addMoney(1);
            }
        }
        while (!deletes.empty()) {
            gamestate->deleteObject(deletes.front());
            deletes.pop_front();
        }
        object->setHeldItem(nullptr);
        gamestate->setDirty(true);
        log->debug("Using an item and killed {}",count);
    }
}

std::string UseEvent::serialize() const
{
    return "UseEvent:"+Event::serialize();
}
