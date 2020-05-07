#pragma once
#include "event.h"
#include "ObjectClasses/movingobject.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class StoppingEvent : public Event {
private:
    glm::mat3 direction;

public:
    StoppingEvent(std::string id, glm::mat3 direction);
    glm::mat3 getDirection();
    virtual void apply(std::shared_ptr<Object> object);
};