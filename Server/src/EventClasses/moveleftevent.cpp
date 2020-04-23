#include "EventClasses/moveleftevent.h"
#include "logger.h"
MoveLeftEvent::MoveLeftEvent(int objectId) : Event(objectId) {

}

void MoveLeftEvent::apply(Object& object) {
	auto log = getLogger("MoveLeftEvent");
	log->trace("{}", object.serialize()); //placeholder so compiler doesn't complain
}

float MoveLeftEvent::getMovementDirection() {
	return -90; //90 degrees to the left //TODO convert to vector
}
