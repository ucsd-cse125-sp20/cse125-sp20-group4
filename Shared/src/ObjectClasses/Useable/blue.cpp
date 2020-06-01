#include "ObjectClasses/Useable/blue.h"

BlueObject::BlueObject():UseableObject(ItemType::BLUE){}
std::string BlueObject::serialize() const {
    return "BlueObject:";
}