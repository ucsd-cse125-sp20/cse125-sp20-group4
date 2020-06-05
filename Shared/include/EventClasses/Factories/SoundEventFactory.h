#ifndef SOUND_EVENT_FACTORY_H
#define SOUND_EVENT_FACTORY_H

#include "EventClasses/Factories/eventfactory.h"

class SoundEventFactory : public IEventFactory {

    std::shared_ptr<Event> create( std::string serial );

};

#endif