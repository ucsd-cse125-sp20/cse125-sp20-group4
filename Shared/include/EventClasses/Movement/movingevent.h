#pragma once
#include "EventClasses/event.h"
#include "ObjectClasses/movingobject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MovingEvent : public Event {
private:
    glm::mat3 direction;

public:
    MovingEvent(std::string id, glm::mat3 direction);
    glm::mat3 getDirection();
    virtual void apply(std::shared_ptr<Object> object);
};