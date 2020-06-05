#pragma once
#include "objectfactory.h"

class SpawnPointFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};