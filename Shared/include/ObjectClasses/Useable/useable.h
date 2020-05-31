#pragma once
#include "ObjectClasses/object.h"
#include <memory>

enum class ItemType { RED=0, GREEN=1, BLUE=2 };
class UseableObject : public Object
{
public:
	UseableObject(ItemType type);
	ItemType itemType;
};