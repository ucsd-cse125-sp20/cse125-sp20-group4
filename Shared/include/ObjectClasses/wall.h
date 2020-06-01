#pragma once
#include "ObjectClasses/object.h"

class Wall : public Object {
	public:

		Wall(std::string& id, float xPos, float yPos, float zPos);

		std::shared_ptr<Object> clone() const;

		//(de)serialize functions
		virtual std::string serialize() const;

		const std::string TAG = "Wall";
		const std::string& getTag();
};