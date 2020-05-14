#include "EventClasses/mouseevent.h"

MouseEvent::MouseEvent(std::string id, glm::vec3 direction) : Event(id)
{
    this->direction = direction;
}

glm::vec3 MouseEvent::getDirection()
{
    return this->direction;
}

void MouseEvent::apply(std::shared_ptr<Object> object)
{
    object->setOrientation(this->getDirection());
}

std::string MouseEvent::serialize()
{
    return "MouseEvent:" + Event::serialize() + "," + std::to_string(this->getDirection().x) + "," + std::to_string(this->getDirection().y) + "," + std::to_string(this->getDirection().z);
}
