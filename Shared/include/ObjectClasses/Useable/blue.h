#pragma once
#include "useable.h"
class BlueObject : public UseableObject
{
public:
	BlueObject();
	std::string serialize() const;
	const std::string TAG = "Blue";
	const std::string& getTag();
};