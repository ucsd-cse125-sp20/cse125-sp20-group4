#include "EventClasses/movingevent.h"

MovingEvent::MovingEvent(int id, glm::mat3 direction) : Event(id) {
    this->direction = direction;
}
glm::mat3 MovingEvent::getDirection() {
    return this->direction;
}
void MovingEvent::apply(Object& object) {
    dynamic_cast<MovingObject&>(object).setVelocity(this->getDirection() * object.getOrientation());
}