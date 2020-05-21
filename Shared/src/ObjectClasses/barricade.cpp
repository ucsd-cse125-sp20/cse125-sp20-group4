#include "ObjectClasses/barricade.h"
#include "logger.h"

Barricade::Barricade(std::string& id, float xPos, float yPos, float zPos, float width, float height, float length, int durability) : Object(id, xPos, yPos, zPos, 1.0f, 0.0f, 0.0f, width, height, length, true){
	setUp(true);
	setDurability(durability);
}

Barricade::Barricade(std::string& id, float xPos, float yPos, float zPos, float width, float height, float length, int durability, bool up) : Object(id, xPos, yPos, zPos, 1.0f, 0.0f, 0.0f, width, height, length, true) {
	setUp(up);
	setDurability(durability);
}


bool Barricade::isUp() const {
	return this->up;
}

void Barricade::toggleUp() {
	this->setUp(!this->isUp());
}

void Barricade::setUp(bool upValue) {
	auto log = getLogger("Barricade");
	this->up = upValue;
	this->setCanCollide(upValue);
	log->trace("Set barricade with id {} to status {}", this->getId(), this->isUp());
}

std::string Barricade::serialize() const {
	auto log = getLogger("Barricade");
	std::string res = "Barricade:"
		+ Object::serialize() + ","
		+ std::to_string(this->isUp()) + ","
		+ std::to_string(this->getDurability());
	log->trace("Serialized Barricade as {}", res);
	return res;
}

int Barricade::getDurability() const{
	return this->durability;
}

void Barricade::degradeBarricade() {
	this->setDurability(this->getDurability() - this->degradeFactor);
}

void Barricade::setDurability(int newDurability) {
	auto log = getLogger("Barricade");
	this->durability = newDurability;
	log->trace("Set durability of barricade with it {} to status {}", this->getId(), newDurability);
	if (this->durability <= 0) {
		this->setUp(false);
	}
	else {
		this->setUp(true);
	}
}