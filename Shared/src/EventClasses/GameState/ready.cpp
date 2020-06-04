#include "EventClasses/GameState/ready.h"
#include "logger.h"
const std::string ReadyEvent::TAG = "ReadyEvent";
ReadyEvent::ReadyEvent(std::string id) : GameStateEvent(id) {}

void ReadyEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("ReadyEvent");
    if(gamestate->phase->state == READY_STATE || gamestate->phase->state == START_STATE){
        std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(gamestate->getObject(this->getObjectId()));
        if (!player->ready) {
            gamestate->phase->count++;
            player->ready = true;
            player->dirty = true;
            gamestate->setDirty(true);
            gamestate->phase->dirty = true;
            log->trace("Set player ready to true");
        }
    }
}

std::string ReadyEvent::serialize() const {
    return TAG + ":" + Event::serialize();
}