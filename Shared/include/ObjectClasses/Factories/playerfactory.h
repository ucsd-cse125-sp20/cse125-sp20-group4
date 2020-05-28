#pragma once
#include "objectfactory.h"
#include "ObjectClasses/player.h"

class PlayerFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};