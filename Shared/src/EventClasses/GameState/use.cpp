#include "EventClasses\GameState\use.h"
#include "logger.h"
#include <glm/glm.hpp>
UseEvent::UseEvent(std::string id) : GameStateEvent(id) {}

void UseEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("UseEvent");
    // get player
    std::shared_ptr<Player> object = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
    // check if found
    if (object != nullptr && object->getHeldItem() != nullptr && std::dynamic_pointer_cast<Barricade>(object->getHeldItem())==nullptr) {
        // get item in player inventory
        std::shared_ptr<Object> item = object->getHeldItem();
        // TODO: determine how to use event

        // loop over gamestate and remove all matching over add money for each removed
        for (auto it = gamestate->getGameObjects().begin(); it != gamestate->getGameObjects().end(); it++) {
            if (std::dynamic_pointer_cast<Enemy>(it->second) != nullptr && glm::distance(object->getPosition(),it->second->getPosition())<5.0f) {
                gamestate->deleteObject(it->second->getId());
                object->setMoney(object->getMoney() + 1);
            }
        }
    }
}

std::string UseEvent::serialize() const
{
    return "UseEvent:"+Event::serialize();
}
