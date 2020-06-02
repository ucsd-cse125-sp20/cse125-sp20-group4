#pragma once
#include "objectfactory.h"
#include "ObjectClasses/shelf.h"

class ShelfFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};