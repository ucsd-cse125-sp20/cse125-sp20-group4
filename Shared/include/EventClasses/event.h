#pragma once
#include <string>
#include "ObjectClasses/object.h"
#include <memory>
class Event
{
private:
	std::string objectId;
public:
	Event(std::string objectId);
	std::string getObjectId();
	virtual std::string serialize();
	virtual void apply(std::shared_ptr<Object> object) = 0;
};