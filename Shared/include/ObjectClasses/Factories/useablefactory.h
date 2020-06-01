#pragma once
#include "objectfactory.h"

class RedItemFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};

class GreenItemFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};

class BlueItemFactory : public IObjectFactory
{
public:
	std::shared_ptr<Object> create(std::string serial);
	std::shared_ptr<Object> create();
};