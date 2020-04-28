#pragma once
#include "event.h"
#include "ObjectClasses/movingobject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class MovingEvent : Event {
private:
    glm::mat3 direction;

public:
    MovingEvent(int id, glm::mat3 direction);
    glm::mat3 getDirection();
    virtual void apply(Object& object);
};