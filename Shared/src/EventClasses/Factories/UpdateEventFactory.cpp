#include "EventClasses/Factories/UpdateEventFactory.h"
#include "EventClasses/UpdateEvent.h"

std::shared_ptr<Event> UpdateEventFactory::create( std::string serial ) {

    return UpdateEvent::deserialize( serial );

}