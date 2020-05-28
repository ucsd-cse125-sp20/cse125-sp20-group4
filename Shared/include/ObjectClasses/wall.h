#pragma once
#include "ObjectClasses/object.h"

class Wall : public Object {
	public:

		Wall(std::string& id, float xPos, float yPos, float zPos);

		//(de)serialize functions
		virtual std::string serialize() const;
		
};