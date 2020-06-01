#include "EventClasses/GameState/ready.h"
#include "logger.h"
const std::string ReadyEvent::TAG = "ReadyEvent";
ReadyEvent::ReadyEvent(std::string id) : GameStateEvent(id) {}

void ReadyEvent::apply(GameState* gamestate) const
{
    auto log = getLogger("PlaceEvent");
    gamestate->makeDirty();
}

std::string ReadyEvent::serialize() const {
    return TAG + ":" + Event::serialize();
}