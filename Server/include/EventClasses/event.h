#pragma once
#include <string>
#include "ObjectClasses/object.h"
class Event
{
private:
	int objectId;
public:
	Event(int objectId);
	int getObjectId();
	std::string serialize();
	virtual void apply(Object& object);
};