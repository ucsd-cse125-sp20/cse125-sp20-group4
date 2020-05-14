#include "EventClasses/movingevent.h"
#include <stdio.h>
#include "glm/gtc/epsilon.hpp"

#define BASE_MOVEMENT 5
float epsilon1 = 0.0005f;

MovingEvent::MovingEvent(std::string id, glm::mat3 direction) : Event(id) {
    this->direction = direction;
}
glm::mat3 MovingEvent::getDirection() {
    return this->direction;
}
void MovingEvent::apply(std::shared_ptr<Object> object) {
    glm::vec3 newVel = (std::dynamic_pointer_cast<MovingObject>(object))->getVelocity() + this->getDirection() * object->getOrientation();
    if (glm::all(epsilonEqual(newVel, glm::vec3(0, 0, 0), epsilon1))) {
        (std::dynamic_pointer_cast<MovingObject>(object))->setVelocity(glm::vec3(0, 0, 0));
    }
    else {
        (std::dynamic_pointer_cast<MovingObject>(object))->setVelocity(normalize(newVel));
    }
}