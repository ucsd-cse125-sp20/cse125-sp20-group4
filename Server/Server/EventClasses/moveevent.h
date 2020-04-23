#pragma once
#include "event.h"
class MoveEvent : public Event
{
public:
	MyClass();
	void apply(Object& object);
	virtual float getMovementDirection();
private:
	const float moveVelocity = 15; //some magic number
};


MoveEvent::apply(Object& object){
	Player target = (Player) object.getObject(this->getObjectId()); //missing some check here
	target.setVelocityX( Math.cos(target.getOrientationY + this->getMovementDirection()) * moveVelocity)); //TODO: X and Z may be switched with respect to cos and sin
	target.setVelocityZ( Math.sin(target.getOrientationY + this->getMovementDirection()) * moveVelocity)); //TODO: convert to vector and move to .cpp file
}