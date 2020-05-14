#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "event.h"
class MouseEvent : public Event
{
private:
    glm::vec3 direction;
public:
    MouseEvent(std::string id, glm::vec3 direction);
    glm::vec3 getDirection();
    virtual void apply(std::shared_ptr<Object> object);
    std::string serialize();
};