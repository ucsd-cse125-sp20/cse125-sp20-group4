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
    if (object != nullptr) {
        glm::vec3 pos = glm::vec3(object->getPosition());
        pos = pos + object->getOrientation()*2.0f;
        // get item in player inventory
        int itemId = object->inventory;
        std::shared_ptr<Object> nObject = nullptr;
        switch (itemId)
        {
            case 1:
                nObject = std::shared_ptr<Object>(new Barricade("ignore", pos.x, pos.y, pos.z));
                log->info("Trying to place {}", nObject->serialize());
                object->inventory = -1;
            break;
        }
        // TODO replace player with correct class
        if (nObject != nullptr) {
            gamestate->createObject(nObject);
        }
    }
}

std::string PlaceEvent::serialize() const {
    return "PlaceEvent:" + Event::serialize();
}