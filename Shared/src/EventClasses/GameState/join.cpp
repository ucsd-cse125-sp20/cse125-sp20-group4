#include "EventClasses/GameState/join.h"
#include "logger.h"
const std::string JoinEvent::TAG = "JoinEvent";
JoinEvent::JoinEvent(std::string id) : GameStateEvent(id) {}

void JoinEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("JoinEvent");
    log->warn("Player Joined: {}",this->getObjectId());
    //gamestate->addPlayer(this->id);
    gamestate->makeDirty();
}

std::string JoinEvent::serialize() const {
    return TAG + ":" + Event::serialize();
}