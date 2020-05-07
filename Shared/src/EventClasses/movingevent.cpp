#include "EventClasses/movingevent.h"
#define BASE_MOVEMENT 5
MovingEvent::MovingEvent(std::string id, glm::mat3 direction) : Event(id) {
    this->direction = direction;
}
glm::mat3 MovingEvent::getDirection() {
    return this->direction;
}
void MovingEvent::apply(std::shared_ptr<Object> object) {
    (std::dynamic_pointer_cast<MovingObject>(object))->setVelocity(normalize(this->getDirection() * object->getOrientation() + (std::dynamic_pointer_cast<MovingObject>(object))->getVelocity()));
}