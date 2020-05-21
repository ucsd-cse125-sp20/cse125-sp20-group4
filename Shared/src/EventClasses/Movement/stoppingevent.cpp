#include "EventClasses/Movement/stoppingevent.h"
#include "glm/gtc/epsilon.hpp"
#define BASE_MOVEMENT 5

float epsilon = 0.0005f;

StoppingEvent::StoppingEvent(std::string id, glm::mat3 direction) : Event(id)
{
    this->direction = direction;
}
glm::mat3 StoppingEvent::getDirection() {
    return this->direction;
}
void StoppingEvent::apply(std::shared_ptr<Object> object) {
    glm::vec3 newVel = (std::dynamic_pointer_cast<MovingObject>(object))->getVelocity() - this->getDirection() * object->getOrientation();
    if (glm::all(epsilonEqual(newVel, glm::vec3(0, 0, 0), epsilon))) {
        (std::dynamic_pointer_cast<MovingObject>(object))->setVelocity(glm::vec3(0, 0, 0));
    }
    else {
        (std::dynamic_pointer_cast<MovingObject>(object))->setVelocity(normalize(newVel));
    }
}
