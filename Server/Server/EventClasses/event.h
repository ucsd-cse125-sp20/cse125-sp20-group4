#pragma once
#include <string>

class Event
{
private:
	int objectId;
public:
	Event(int objectId);
	int getObjectId();
	std::string serialize();
	void apply(Object& object);
};