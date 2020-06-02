#pragma once
#include "EventClasses/Factories/eventfactory.h"

class DeleteEventFactory : public IEventFactory {

    std::shared_ptr<Event> create(std::string serial);

};