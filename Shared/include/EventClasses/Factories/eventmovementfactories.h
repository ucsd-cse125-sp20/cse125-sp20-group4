#pragma once
#include "eventfactory.h"

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