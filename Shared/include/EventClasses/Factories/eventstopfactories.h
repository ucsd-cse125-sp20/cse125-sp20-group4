#pragma once
#include "eventfactory.h"

class StopLeftEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};

class StopRightEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};

class StopForwardEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};

class StopBackwardEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};