#pragma once
#include "movingevent.h"

class MoveBackwardEvent : public MovingEvent {

	public:
	MoveBackwardEvent( const std::string & objectId );
	std::string serialize() const;

};
