#include "EventClasses/GameState/place.h"
#include "ObjectClasses/barricade.h"
#include "logger.h"

#include <glm/gtc/epsilon.hpp>
#define EPSILON 0.0005f
PlaceEvent::PlaceEvent(std::string id) : GameStateEvent(id) {}

void PlaceEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("PlaceEvent");
    // get object
    std::shared_ptr<Player> object = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
    // check if found
    if (object != nullptr && object->getHeldItem() !=nullptr && object->getHeldItem()->getTag().compare("Barricade")==0) {
        log->info("Placing object: {}", object->getHeldItem()->serialize());
        // get item in player inventory
        //std::shared_ptr<Object> item = object->getHeldItem();
        std::shared_ptr<Barricade> item = std::dynamic_pointer_cast<Barricade>(object->getHeldItem());

        glm::vec3 pos = glm::vec3(object->getPosition());
        glm::vec3 ori = object->getOrientation();
        ori.y = 0;
        if (glm::all(epsilonEqual(ori, glm::vec3(0, 0, 0), EPSILON))) {
            ori = glm::vec3(1.0f, 0.0f, 0.0f);
        } else{
            ori = glm::normalize(ori);
        }
        pos = pos + ori * object->getWidth() * 2.0f;
        gamestate->map->GridifyMapPos(pos);
        //TODO check for collision
        if (!gamestate->map->containsObject(pos)) {
            item->setPosition(pos.x, pos.y, pos.z);
            gamestate->createObject(item);
            object->setHeldItem(nullptr);
            log->info("Just Placed an item");
        }
        
    }
}

std::string PlaceEvent::serialize() const {
    return "PlaceEvent:" + Event::serialize();
}