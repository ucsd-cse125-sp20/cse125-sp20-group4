#pragma once
#include "EventClasses/event.h"
#include <memory>
class IEventFactory
{
public:
    virtual std::shared_ptr<Event> create(std::string serial)=0;

    virtual ~IEventFactory() {};
};
