#pragma once
#include "object.h"
#include "ObjectClasses/Factories/objectfactory.h"
#include <memory>


class Shelf : public Object {
protected:
	std::shared_ptr<IObjectFactory> item;
public:
	static const std::string TAG;
	Shelf();
	Shelf(const Shelf& obj);
	Shelf(const std::string& id, float xPos, float yPos, float zPos, float width = 1.0f, float height = 1.0f, float length = 1.0f, std::shared_ptr<IObjectFactory> shelfItem = nullptr);

	std::shared_ptr<Object> getItem() const;
	std::shared_ptr<IObjectFactory> getFactory() const;
	void setItem(std::shared_ptr<IObjectFactory> newItem);
	//(de)serialize functions
	virtual std::string serialize() const;
	virtual std::shared_ptr<Object> clone() const override;


};