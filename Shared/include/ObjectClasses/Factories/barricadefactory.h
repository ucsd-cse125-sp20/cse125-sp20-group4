#pragma once
#include "objectfactory.h"
#include "ObjectClasses/barricade.h"

class BarricadeFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};