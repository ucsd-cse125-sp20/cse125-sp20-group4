#include "EventClasses/stoppingevent.h"
#include "glm/gtc/epsilon.hpp"

#define BASE_MOVEMENT 5
#define EPSILON 0.0005f

StoppingEvent::StoppingEvent( const std::string & id, const glm::vec3 & direction ) : Event( id ), direction( direction ) {}

const glm::vec3 & StoppingEvent::getDirection() const {

    return this->direction;

}

void StoppingEvent::apply( std::shared_ptr<Object> object ) const {

    std::shared_ptr<MovingObject> obj = std::dynamic_pointer_cast< MovingObject >( object );
    glm::vec3 newVel = obj->getRelativeVelocity() - this->getDirection();
    if ( glm::all( epsilonEqual( newVel, glm::vec3( 0, 0, 0 ), EPSILON ) ) ) {
        obj->setRelativeVelocity( glm::vec3( 0, 0, 0 ) );
    } else {
        obj->setRelativeVelocity( glm::normalize( newVel ) );
    }

}