#ifndef UPDATE_EVENT_FACTORY_H
#define UPDATE_EVENT_FACTORY_H

#include "EventClasses/Factories/eventfactory.h"

class UpdateEventFactory : public IEventFactory {

    std::shared_ptr<Event> create( std::string serial );

};

#endif