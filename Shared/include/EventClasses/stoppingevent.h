#pragma once
#include "event.h"
#include "ObjectClasses/movingobject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class StoppingEvent : public Event {

    private:
    const glm::vec3 direction;

    public:
    StoppingEvent( const std::string & id, const glm::vec3 & direction );
    const glm::vec3 & getDirection() const;
    virtual void apply( std::shared_ptr<Object> object ) const;

};