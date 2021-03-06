#pragma once
#include "EventClasses/Object/objectevent.h"
#include "ObjectClasses/movingobject.h"
#include <glm/glm.hpp>

class MovingEvent : public ObjectEvent {

    private:
    const glm::vec3 direction;

    public:
    MovingEvent( const std::string & id, const glm::vec3 & direction );
    const glm::vec3 & getDirection() const;
    virtual void apply( std::shared_ptr<Object> object ) const;

};