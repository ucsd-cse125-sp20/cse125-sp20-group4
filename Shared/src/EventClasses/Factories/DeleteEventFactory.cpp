#include "EventClasses/Factories/DeleteEventFactory.h"
#include "EventClasses/Server/DeleteEvent.h"
std::shared_ptr<Event> DeleteEventFactory::create(std::string serial) {
    return DeleteEvent::deserialize(serial);
}