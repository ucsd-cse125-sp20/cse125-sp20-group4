#pragma once
#include "EventClasses/Factories/eventfactory.h"

class NotifyEventFactory : public IEventFactory {

    std::shared_ptr<Event> create(std::string serial);

};