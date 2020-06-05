#include "EventClasses/Factories/SoundEventFactory.h"
#include "EventClasses/SoundEvent.h"

std::shared_ptr<Event> SoundEventFactory::create( std::string serial ) {

    return SoundEvent::deserialize( serial );

}