#include "EventClasses/Movement/mouseevent.h"
#include "ObjectClasses/movingobject.h"
#define ROTATE( direction, angle, axis ) ( glm::rotate( glm::mat4( 1.0f ), ( angle ), ( axis ) ) * glm::vec4( ( direction ), 1.0f ) )

MouseEvent::MouseEvent(std::string id, float angle, glm::vec3 axis) : Event(id)
{
    this->axis = axis;
    this->angle = angle;
}


void MouseEvent::apply(std::shared_ptr<Object> object)
{
    //update velocity
    glm::vec3 newDir = ROTATE(object->getOrientation(), this->angle, this->axis);
    auto mobj = (std::dynamic_pointer_cast<MovingObject>(object));
    glm::vec3 newVel = ROTATE(mobj->getVelocity(), this->angle, this->axis);
    object->setOrientation(newDir);
    mobj->setVelocity(newVel);
}

std::string MouseEvent::serialize()
{
    return "MouseEvent:" + Event::serialize() + "," + std::to_string(this->angle) + "," + std::to_string(this->axis.x) + "," + std::to_string(this->axis.y) + "," + std::to_string(this->axis.z);
}
