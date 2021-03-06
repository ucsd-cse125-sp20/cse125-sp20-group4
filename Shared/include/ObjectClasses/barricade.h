#pragma once
#include "object.h"


class Barricade : public Object {
protected:
	mutable bool up;
	mutable int durability;
	const int degradeFactor = 1;
public:
	Barricade();
	Barricade(const Barricade& obj);
	Barricade(std::string id, float xPos, float yPos, float zPos, float width = 1.0f, float height = 1.0f, float length = 1.0f, int durability = 5000, bool up = true);

	const std::string TAG = "Barricade";
	const std::string& getTag();

	bool isUp() const;
	void toggleUp() const;
	void setUp(bool upValue) const;
	int getDurability() const;
	void setDurability(int newDurability) const;
	void degradeBarricade() const;
	virtual std::shared_ptr<Object> clone() const override;

	//(de)serialize functions
	virtual std::string serialize() const;

};