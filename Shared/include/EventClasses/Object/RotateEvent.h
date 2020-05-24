#ifndef ROTATE_EVENT_H
#define ROTATE_EVENT_H

#include <glm/vec3.hpp>

#include "EventClasses/Object/objectevent.h"

class RotateEvent : public ObjectEvent {

    public:
    RotateEvent( const std::string & id, const glm::vec3 & newDirection );

    const glm::vec3 newDirection;

    std::string serialize() const;
    void apply( std::shared_ptr<Object> object ) const;

};

#endif