#include "ObjectClasses/Useable/blue.h"

BlueObject::BlueObject():UseableObject(ItemType::BLUE){}
std::string BlueObject::serialize() const {
    return "BlueObject:";
}
const std::string& BlueObject::getTag() {
    return TAG;
}