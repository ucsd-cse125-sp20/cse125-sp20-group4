#include "moveleftevent.h"
MoveLeftEvent::MoveLeftEvent() {

}

MoveLeftEvent::apply(Object& object) {

}

float MoveLeftEvent::getMovementDirection() {
	return -90; //90 degrees to the left //TODO convert to vector
}
