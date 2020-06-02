#pragma once
#include "object.h"
#include "ObjectClasses/Factories/objectfactory.h"
#include <memory>


class Shelf : public Object {
protected:
	std::shared_ptr<IObjectFactory> item;
public:
	Shelf();
	Shelf(const Shelf& obj);
	Shelf(const std::string& id, float xPos, float yPos, float zPos, float orx, float ory, float orz, float width = 1.0f, float height = 1.0f, float length = 1.0f, std::shared_ptr<IObjectFactory> shelfItem = nullptr, int isCorn = 0);

	const std::string TAG = "Shelf";
	const std::string& getTag();
	int isCorner = 0;
	std::shared_ptr<Object> getItem() const;
	std::shared_ptr<IObjectFactory> getFactory() const;
	void setItem(std::shared_ptr<IObjectFactory> newItem);
	//(de)serialize functions
	virtual std::string serialize() const;
	virtual std::shared_ptr<Object> clone() const override;


};