#include "ObjectClasses/Useable/green.h"

GreenObject::GreenObject() : UseableObject(ItemType::GREEN) {}
std::string GreenObject::serialize() const
{
    return "GreenObject:";
}

const std::string& GreenObject::getTag() {
    return TAG;
}