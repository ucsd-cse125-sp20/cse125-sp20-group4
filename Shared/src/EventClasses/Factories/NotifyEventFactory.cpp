#include "EventClasses/Factories/NotifyEventFactory.h"
#include "EventClasses/Server/NotifyEvent.h"
std::shared_ptr<Event> NotifyEventFactory::create(std::string serial) {
    return NotifyEvent::deserialize(serial);
}