#include "EventClasses/Factories/eventmovementfactories.h"
#include "EventClasses/moveleftevent.h"
#include "EventClasses/moverightevent.h"
#include "EventClasses/moveforwardevent.h"
#include "EventClasses/movebackwardevent.h"

std::shared_ptr<Event> MoveLeftEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<MoveLeftEvent>(new MoveLeftEvent(id));
    return event;
}

std::shared_ptr<Event> MoveRightEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<MoveRightEvent>(new MoveRightEvent(id));
    return event;
}

std::shared_ptr<Event> MoveForwardEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<MoveForwardEvent>(new MoveForwardEvent(id));
    return event;
}

std::shared_ptr<Event> MoveBackwardEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<MoveBackwardEvent>(new MoveBackwardEvent(id));
    return event;
}