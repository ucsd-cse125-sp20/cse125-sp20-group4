#ifndef ROTATE_EVENT_H
#define ROTATE_EVENT_H

#include <glm/vec3.hpp>

#include "event.h"

class RotateEvent : public Event {

    public:
    RotateEvent( const std::string & id, const glm::vec3 & newDirection );

    const glm::vec3 newDirection;

    std::string serialize() const;
    void apply( std::shared_ptr<Object> object ) const;

};

#endif