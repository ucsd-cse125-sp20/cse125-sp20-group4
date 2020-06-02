#include "EventClasses/GameState/gamestateevent.h"

GameStateEvent::GameStateEvent(std::string id) : Event(id, Event::EventType::GEvent) {}
