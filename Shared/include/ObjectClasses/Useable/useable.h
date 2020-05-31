#pragma once
#include "ObjectClasses/object.h"
#include <memory>

enum class ItemType { RED, GREEN, BLUE };
class UseableObject : public Object
{
public:
	UseableObject(ItemType type);
	ItemType itemType;
};