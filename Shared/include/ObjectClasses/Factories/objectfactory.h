#pragma once
#include "ObjectClasses/object.h"
#include <memory>
class IObjectFactory
{
public:
	virtual std::shared_ptr<Object> create(std::string serial) = 0;
	virtual ~IObjectFactory() {};
};