#ifndef ROTATE_FACTORY_H
#define ROTATE_FACTORY_H

#include "eventfactory.h"

class RotateEventFactory : public IEventFactory {

	public:
	std::shared_ptr<Event> create( std::string serial );

};

#endif