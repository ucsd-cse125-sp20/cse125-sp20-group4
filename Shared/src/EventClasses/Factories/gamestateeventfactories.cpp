#include "EventClasses/Factories/gamestateeventfactories.h"
#include "EventClasses/GameState/place.h"
#include "EventClasses/GameState/pickup.h"
#include "EventClasses/GameState/ready.h"

#include "EventClasses/GameState/join.h"
std::shared_ptr<Event> PlaceEventFactory::create(std::string serial)
{
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<PlaceEvent>(new PlaceEvent(id));
    return event;
}

std::shared_ptr<Event> PickUpEventFactory::create(std::string serial)
{
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    // target id
    last = pos + 1;
    pos = serial.find(",", last);
    std::string targetId = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<PickUpEvent>(new PickUpEvent(id,targetId));
    return event;
}

std::shared_ptr<Event> ReadyEventFactory::create(std::string serial)
{
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<ReadyEvent>(new ReadyEvent(id));
    return event;
}

std::shared_ptr<Event> JoinEventFactory::create(std::string serial)
{
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<JoinEvent>(new JoinEvent(id));
    return event;
}