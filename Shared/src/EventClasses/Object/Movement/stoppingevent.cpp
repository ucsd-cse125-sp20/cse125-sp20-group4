#include "EventClasses/Object/Movement/stoppingevent.h"
#include "glm/gtc/epsilon.hpp"
#include "logger.h"
#define BASE_MOVEMENT 5
#define EPSILON 0.0005f

StoppingEvent::StoppingEvent( const std::string & id, const glm::vec3 & direction ) : ObjectEvent( id ), direction( direction ) {}

const glm::vec3 & StoppingEvent::getDirection() const {

    return this->direction;

}

void StoppingEvent::apply( std::shared_ptr<Object> object ) const {
    auto log = getLogger("Event");
    std::shared_ptr<MovingObject> obj = std::dynamic_pointer_cast< MovingObject >( object );
    glm::vec3 newVel = obj->getRelativeVelocity() - this->getDirection();
    bool isValid = newVel.x<2.0 && newVel.x>-2.0 && newVel.y<2.0 && newVel.y>-2.0 && newVel.z<2.0 && newVel.z>-2.0;
    if (isValid) {
        log->info("Applying {} with direction ({}, {}, {}) Old rel velocity was ({}, {}, {}) and New is ({}, {}, {})", this->serialize(), this->direction.x, this->direction.y, this->direction.z, obj->getRelativeVelocity().x, obj->getRelativeVelocity().y, obj->getRelativeVelocity().z, newVel.x, newVel.y, newVel.z);
        if (glm::all(epsilonEqual(newVel, glm::vec3(0, 0, 0), EPSILON))) {
            obj->setRelativeVelocity(glm::vec3(0, 0, 0));
        } else {
            obj->setRelativeVelocity(newVel);
        }
    }

}