#include "EventClasses/stoppingevent.h"
#define BASE_MOVEMENT 5
StoppingEvent::StoppingEvent(std::string id, glm::mat3 direction) : Event(id)
{
    this->direction = direction;
}
glm::mat3 StoppingEvent::getDirection() {
    return this->direction;
}
void StoppingEvent::apply(std::shared_ptr<Object> object) {
    (std::dynamic_pointer_cast<MovingObject>(object))->setVelocity(normalize((std::dynamic_pointer_cast<MovingObject>(object))->getVelocity()- this->getDirection() * object->getOrientation()));
}
