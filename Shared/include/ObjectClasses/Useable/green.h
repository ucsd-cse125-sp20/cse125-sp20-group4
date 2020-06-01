#pragma once
#include "useable.h"
class GreenObject : public UseableObject
{
public:
	GreenObject();
	std::string serialize() const;
	const std::string TAG = "Green";
	const std::string& getTag();
};