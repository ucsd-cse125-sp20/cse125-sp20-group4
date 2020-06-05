#include "phases/updatephasefactory.h"
#include "phases/phasefactory.h"
#include "phases/updatephaseevent.h"

std::shared_ptr<Event> UpdatePhaseFactory::create(std::string serial)
{
    size_t pos = serial.find(":");
    size_t last = pos + 1;
    std::string tag = serial.substr(0, pos);
    
    std::shared_ptr<Phase> tmp = PhaseFactory::create(serial.substr(pos + 1));

    std::shared_ptr<Event> event = std::make_shared<UpdatePhaseEvent>(tmp);
    return event;
}
