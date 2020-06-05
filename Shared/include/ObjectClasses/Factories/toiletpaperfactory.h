#pragma once
#include "objectfactory.h"

class ToiletPaperFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};