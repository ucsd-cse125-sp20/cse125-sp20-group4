#pragma once
#include "useable.h"
class BlueObject : public UseableObject
{
public:
	BlueObject();
	std::string serialize() const;
};