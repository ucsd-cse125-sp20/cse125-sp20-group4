#pragma once
#include "object.h"


class Barricade : public Object {
protected:
	bool up;
	int durability;
	const int degradeFactor = 10;
public:
	Barricade(std::string& id, float xPos, float yPos, float zPos, float width = 1.0f, float height = 1.0f, float length = 1.0f, int durability = 100);
	Barricade(std::string& id, float xPos, float yPos, float zPos, float width = 1.0f, float height = 1.0f, float length = 1.0f, int durability = 100, bool up = false);

	bool isUp() const;
	void toggleUp();
	void setUp(bool upValue);
	int getDurability() const;
	void setDurability(int newDurability);
	void degradeBarricade();
	//(de)serialize functions
	virtual std::string serialize() const;

};