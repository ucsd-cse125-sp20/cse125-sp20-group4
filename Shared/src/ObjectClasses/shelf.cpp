#include "ObjectClasses/shelf.h"
#include "logger.h"

Shelf::Shelf(std::string& id, float xPos, float yPos, float zPos, float width, float height, float length) : Object(id, xPos, yPos, zPos, 1.0f, 0.0f, 0.0f, width, height, length, true) {
	setItem(nullptr);
}

Shelf::Shelf(std::string& id, float xPos, float yPos, float zPos, float width, float height, float length, std::shared_ptr<Object> shelfItem) : Object(id, xPos, yPos, zPos, 1.0f, 0.0f, 0.0f, width, height, length, true) {
	setItem(shelfItem);
}

std::shared_ptr<Object> Shelf::getItem() const{
	return item;
}
void Shelf::setItem(std::shared_ptr<Object> newItem) {
	this->item = newItem;
}
//(de)serialize functions
std::string Shelf::serialize() const {
	auto log = getLogger("Shelf");
	std::string res = "Shelf:" + Object::serialize();

	if (this->getItem() == nullptr) {
		res += "noItem/";
	}
	else {
		res += this->getItem()->serialize() + "/";
	}
	log->trace("Serialized Shelf as {}", res);
	return res;
}