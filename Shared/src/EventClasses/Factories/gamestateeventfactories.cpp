#include "EventClasses/Factories/gamestateeventfactories.h"
#include "EventClasses/GameState/place.h"
std::shared_ptr<Event> PlaceEventFactory::create(std::string serial)
{
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);

    std::shared_ptr<Event> event = std::shared_ptr<PlaceBarricadeEvent>(new PlaceBarricadeEvent(id));
    return event;
}
