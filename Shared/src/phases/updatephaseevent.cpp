#include "phases/updatephaseevent.h"
UpdatePhaseEvent::UpdatePhaseEvent(std::shared_ptr<Phase> phase) : Event("ignore", Event::EventType::PEvent) {
    this->phase.count = phase->count;
    this->phase.health = phase->health;
    this->phase.wave = phase->wave;
    this->phase.state = phase->state;
}

std::string UpdatePhaseEvent::serialize() const
{
    return "PhaseUpdate:" + phase.serialize()+"|";
}
