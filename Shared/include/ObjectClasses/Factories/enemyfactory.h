#pragma once
#include "objectfactory.h"
#include "ObjectClasses/enemy.h"

class EnemyFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};