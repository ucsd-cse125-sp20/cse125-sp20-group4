#include "ObjectClasses/Useable/red.h"

RedObject::RedObject() : UseableObject(ItemType::RED){}
std::string RedObject::serialize() const{
    return "RedObject:";
}

const std::string& RedObject::getTag() {
    return TAG;
}

std::shared_ptr<Object> RedObject::clone() const {
    return std::make_shared<RedObject>();
}