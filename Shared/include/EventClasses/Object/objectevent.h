#pragma once
#include "ObjectClasses/object.h"
#include "EventClasses/event.h"
class ObjectEvent : public Event
{
public:
	ObjectEvent(const std::string objectId);
	virtual void apply(std::shared_ptr<Object> object) const = 0;
};
