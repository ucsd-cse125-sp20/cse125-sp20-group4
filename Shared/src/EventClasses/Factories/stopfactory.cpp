
#include "EventClasses/Factories/eventstopfactories.h"
#include "EventClasses/stopleftevent.h"
#include "EventClasses/stoprightevent.h"
#include "EventClasses/stopforwardevent.h"
#include "EventClasses/stopbackwardevent.h"


std::shared_ptr<Event> StopLeftEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<StopLeftEvent>(new StopLeftEvent(id));
    return event;
}

std::shared_ptr<Event> StopRightEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<StopRightEvent>(new StopRightEvent(id));
    return event;
}

std::shared_ptr<Event> StopForwardEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<StopForwardEvent>(new StopForwardEvent(id));
    return event;
}

std::shared_ptr<Event> StopBackwardEventFactory::create(std::string serial) {
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<StopBackwardEvent>(new StopBackwardEvent(id));
    return event;
}