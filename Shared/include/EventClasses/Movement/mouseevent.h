#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "EventClasses/event.h"
class MouseEvent : public Event
{
public:
    float angle;
    glm::vec3 axis;
    MouseEvent(std::string id, float angle, glm::vec3 axis);
    virtual void apply(std::shared_ptr<Object> object);
    std::string serialize();
};