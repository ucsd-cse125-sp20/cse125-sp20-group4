#pragma once
#include "EventClasses/Factories/eventfactory.h"
class UpdatePhaseFactory : public IEventFactory
{
    std::shared_ptr<Event> create(std::string serial);

};