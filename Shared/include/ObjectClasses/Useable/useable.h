#pragma once
#include "ObjectClasses/object.h"
class UseableObject : public Object
{
public:
	UseableObject(std::string id, UseableObject::Type type);
	~UseableObject();

private:
	enum class Type {RED, GREEN, BLUE};
};