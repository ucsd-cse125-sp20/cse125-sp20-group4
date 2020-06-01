#pragma once
#include "useable.h"
class RedObject : public UseableObject
{
public:
	RedObject();
	std::string serialize() const;
	const std::string TAG = "Red";
	const std::string& getTag();
	std::shared_ptr<Object> clone() const;
};