#pragma once
#include "ObjectClasses/object.h"
class UseableObject : public Object
{
public:
	enum class ItemType { RED, GREEN, BLUE };
	UseableObject(std::string id, UseableObject::ItemType type);
	~UseableObject();
};