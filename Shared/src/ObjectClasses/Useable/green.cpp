#include "ObjectClasses/Useable/green.h"

GreenObject::GreenObject() : UseableObject(ItemType::GREEN) {}
std::string GreenObject::serialize() const
{
    return "GreenObject:";
}

const std::string& GreenObject::getTag() {
    return TAG;
}

std::shared_ptr<Object> GreenObject::clone() const {
    return std::make_shared<GreenObject>();
}