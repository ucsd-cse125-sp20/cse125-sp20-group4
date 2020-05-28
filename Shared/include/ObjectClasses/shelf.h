#pragma once
#include "object.h"
#include <memory>


class Shelf : public Object {
protected:
	std::shared_ptr<Object> item;
public:
	Shelf(std::string& id, float xPos, float yPos, float zPos, float width = 1.0f, float height = 1.0f, float length = 1.0f);
	Shelf(std::string& id, float xPos, float yPos, float zPos, float width = 1.0f, float height = 1.0f, float length = 1.0f, std::shared_ptr<Object> shelfItem = nullptr);

	std::shared_ptr<Object> getItem() const;
	void setItem(std::shared_ptr<Object> newItem);
	//(de)serialize functions
	virtual std::string serialize() const;

};