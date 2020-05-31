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
    if (object != nullptr && object->getHeldItem() != nullptr && std::dynamic_pointer_cast<Barricade>(object->getHeldItem())==nullptr) {
        // get item in player inventory
        std::shared_ptr<UseableObject> item = std::dynamic_pointer_cast<UseableObject>(object->getHeldItem());
        // TODO: determine how to use event
        ItemType itype = item->itemType;
        log->warn("Using an item");
        // loop over gamestate and remove all matching over add money for each removed
        std::string id = "";
        for (auto it = gamestate->getGameObjects().begin(); it != gamestate->getGameObjects().end();) {
            if (id.compare("") != 0) {
                gamestate->deleteObject(id);
                id = "";
            }
            auto enemy = std::dynamic_pointer_cast<Enemy>(it->second);
            if ( enemy != nullptr && glm::distance(object->getPosition(),enemy->getPosition())<5.0f && enemy->weakness == itype) {
                id = it->second->getId();
                object->addMoney(1);
            }
        }
    }
}

std::string UseEvent::serialize() const
{
    return "UseEvent:"+Event::serialize();
}
