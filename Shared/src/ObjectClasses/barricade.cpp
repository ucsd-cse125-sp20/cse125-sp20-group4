#include "ObjectClasses/barricade.h"
#include "logger.h"

const std::string& Barricade::getTag() {
	return TAG;
}
Barricade::Barricade(const Barricade& obj): Barricade(obj.getId(), obj.getPositionX(), obj.getPositionY(), obj.getPositionZ(), obj.getWidth(), obj.getHeight(), obj.getLength(), obj.getDurability(), obj.isUp()) {}

Barricade::Barricade() : Barricade("ignore", 0.0f, 0.0f, 0.0f, 1, 1, 1) {}

Barricade::Barricade(std::string id, float xPos, float yPos, float zPos, float width, float height, float length, int durability, bool up) : Object(id, xPos, yPos, zPos, 1.0f, 0.0f, 0.0f, width, height, length, up) {
	setUp(up);
	setDurability(durability);
}


bool Barricade::isUp() const {
	return this->up;
}

void Barricade::toggleUp() const {
	this->setUp(!this->isUp());
}

void Barricade::setUp(bool upValue) const {
	auto log = getLogger(TAG);
	this->up = upValue;
	this->setCanCollide(upValue);
	this->dirty = true;
	log->trace("Set barricade with id {} to status {}", this->getId(), this->isUp());
}

std::string Barricade::serialize() const {
	auto log = getLogger(TAG);
	std::string res = TAG+":"
		+ Object::serialize() + ","
		+ std::to_string(this->isUp()) + ","
		+ std::to_string(this->getDurability());
	log->trace("Serialized Barricade as {}", res);
	return res;
}

int Barricade::getDurability() const{
	return this->durability;
}

void Barricade::degradeBarricade() const {
	auto log = getLogger(TAG);
	this->setDurability(this->getDurability() - this->degradeFactor);
	this->dirty = true;
}

void Barricade::setDurability(int newDurability) const {
	auto log = getLogger(TAG);
	this->durability = newDurability;
	log->trace("Set durability of barricade with id {} to {}", this->getId(), newDurability);
	if (this->durability <= 0) {
		this->setUp(false);
	}
	else {
		this->setUp(true);
	}
	this->dirty = true;
}

std::shared_ptr<Object> Barricade::clone() const {
	return std::make_shared<Barricade>(*this);
}
