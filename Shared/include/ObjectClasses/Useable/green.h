#pragma once
#include "useable.h"
class GreenObject : public UseableObject
{
public:
	GreenObject();
	std::string serialize() const;
};