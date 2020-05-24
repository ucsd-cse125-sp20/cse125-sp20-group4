#include "EventClasses/Object/RotateEvent.h"

RotateEvent::RotateEvent( const std::string & id, const glm::vec3 & newDirection ) : ObjectEvent( id ), newDirection( glm::normalize( newDirection ) ) {}

std::string RotateEvent::serialize() const {

    return "RotateEvent:" + Event::serialize() + "," + std::to_string( newDirection.x ) + "," + std::to_string( newDirection.y ) + "," + std::to_string( newDirection.z );

}

void RotateEvent::apply( std::shared_ptr<Object> object ) const {

    object->setOrientation( newDirection );

}