#include "ObjectClasses/Useable/red.h"

RedObject::RedObject() : UseableObject(ItemType::RED){}
std::string RedObject::serialize() const{
    return "RedObject:";
}