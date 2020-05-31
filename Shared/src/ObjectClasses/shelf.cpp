#include "ObjectClasses/shelf.h"
#include "ObjectClasses/Factories/objectfactory.h"
#include "logger.h"
const std::string Shelf::TAG = "Shelf";
Shelf::Shelf() : Shelf(std::string("ignore"), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f) {}

Shelf::Shelf(const Shelf& obj): Shelf(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(), obj.getWidth(), obj.getHeight(), obj.getLength(), obj.getFactory()){}


Shelf::Shelf(const std::string& id, float xPos, float yPos, float zPos, float width, float height, float length, std::shared_ptr<IObjectFactory> shelfItem) : Object(id, xPos, yPos, zPos, 1.0f, 0.0f, 0.0f, width, height, length, true) {
	setItem(shelfItem);
}

std::shared_ptr<Object> Shelf::getItem() const{
	return item->create();
}

std::shared_ptr<IObjectFactory> Shelf::getFactory() const {
	return item;
}


void Shelf::setItem(std::shared_ptr<IObjectFactory> newItem) {
	this->item = newItem;
	this->dirty = true;
}
//(de)serialize functions
std::string Shelf::serialize() const {
	auto log = getLogger(TAG);
	std::string res = TAG + ":" + Object::serialize();
	/*
	if (this->getItem() == nullptr) {
		res += "noItem/";
	}
	else {
		res += this->getItem()->serialize() + "/";
	}*/
	log->trace("Serialized Shelf as {}", res);
	return res;
}

std::shared_ptr<Object> Shelf::clone() const {
	return std::make_shared<Shelf>(*this);
}
