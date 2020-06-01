#include "ObjectClasses/shelf.h"
#include "ObjectClasses/Factories/objectfactory.h"
#include "logger.h"

const std::string& Shelf::getTag() {
	return TAG;
}

Shelf::Shelf() : Shelf(std::string("ignore"), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f) {}

Shelf::Shelf(const Shelf& obj): Shelf(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(),obj.getOrientationX(), obj.getOrientationY(), obj.getOrientationZ(), obj.getWidth(), obj.getHeight(), obj.getLength(), obj.getFactory(), obj.isCorner){}


Shelf::Shelf(const std::string& id, float xPos, float yPos, float zPos, float orx, float ory, float orz, float width, float height, float length, std::shared_ptr<IObjectFactory> shelfItem, int isCorn) : Object(id, xPos, yPos, zPos, orx, ory, orz, width, height, length, true) {
	setItem(shelfItem);
	isCorner = isCorn;
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
	std::string res = TAG + ":" + Object::serialize() +","+ std::to_string(isCorner) +",";
	
	if (this->getFactory() == nullptr) {
		res += "noItem/";
	}
	else {
		res += this->getItem()->getTag() + "/";
	}
	log->trace("Serialized Shelf as {}", res);
	return res;
}

std::shared_ptr<Object> Shelf::clone() const {
	return std::make_shared<Shelf>(*this);
}
