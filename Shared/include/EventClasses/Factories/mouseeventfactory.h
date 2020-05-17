#pragma once
#include "eventfactory.h"
class MouseEventFactory : public IEventFactory
{
public:
	std::shared_ptr<Event> create(std::string serial);
};