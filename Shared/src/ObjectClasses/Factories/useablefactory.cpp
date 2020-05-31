#include "ObjectClasses/Factories/useablefactory.h"
#include "ObjectClasses/Useable/red.h"
#include "ObjectClasses/Useable/blue.h"
#include "ObjectClasses/Useable/green.h"
std::shared_ptr<Object> RedItemFactory::create(std::string serial)
{
    return create();
}

std::shared_ptr<Object> RedItemFactory::create()
{
    return std::make_shared<RedObject>();
}

std::shared_ptr<Object> GreenItemFactory::create(std::string serial)
{
    return create();
}

std::shared_ptr<Object> GreenItemFactory::create()
{
    return std::make_shared<GreenObject>();
}

std::shared_ptr<Object> BlueItemFactory::create(std::string serial)
{
    return create();
}

std::shared_ptr<Object> BlueItemFactory::create()
{
    return std::make_shared<BlueObject>();
}
