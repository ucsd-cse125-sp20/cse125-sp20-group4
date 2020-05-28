#pragma once
#include "eventfactory.h"
class PlaceEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};

class PickUpEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};