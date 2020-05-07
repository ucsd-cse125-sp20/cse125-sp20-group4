#include "EventClasses/stopleftevent.h"

StopLeftEvent::StopLeftEvent(std::string objectId) : StoppingEvent(objectId, glm::mat3(0.0f,-1.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)){}
std::string StopLeftEvent::serialize() {
    return "StopLeft:" + Event::serialize();
}