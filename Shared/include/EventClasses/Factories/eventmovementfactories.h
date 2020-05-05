#pragma once
#include "eventfactory.h"
#include "EventClasses/moveleftevent.h"
#include "EventClasses/moverightevent.h"
#include "EventClasses/moveforwardevent.h"
#include "EventClasses/movebackwardevent.h"

class MoveLeftEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};

class MoveRightEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};

class MoveForwardEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};

class MoveBackwardEventFactory : public IEventFactory {
public:
    std::shared_ptr<Event> create(std::string serial);
};