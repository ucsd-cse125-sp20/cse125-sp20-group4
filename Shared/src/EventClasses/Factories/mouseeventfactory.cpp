#include "EventClasses/Factories/mouseeventfactory.h"
#include "EventClasses/mouseevent.h"
std::shared_ptr<Event> MouseEventFactory::create(std::string serial)
{
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    // id
    pos = serial.find(",", last);
    std::string id = serial.substr(last, pos - last);
    // x
    last = pos + 1;
    pos = serial.find(",", last);
    float orx = std::stof(serial.substr(last, pos - last));
    // y
    last = pos + 1;
    pos = serial.find(",", last);
    float ory = std::stof(serial.substr(last, pos - last));
    // z
    last = pos + 1;
    pos = serial.find(",", last);
    float orz = std::stof(serial.substr(last, pos - last));

    std::shared_ptr<Event> event = std::shared_ptr<MouseEvent>(new MouseEvent(id,glm::vec3(orx,ory,orz)));
    return event;
}
