#pragma once

#include "glm/glm.hpp"

#include <list>
#include <queue>
#include <unordered_map>

#include "ObjectClasses/objects.h"


class MapRep {

	struct xy_coord {
		int x, z;
	};

public:
	
	void addObject(Object * object, glm::vec3 pos);
	void removeObject(glm::vec3 pos);
	bool containsObject(glm::vec3 pos);

	std::list<glm::vec3> getPath(glm::vec3 start, glm::vec3 end);

private:
	int map[][];
	int height, width;
	MapRep(int height = 100, int width = 100) : height(height), width(width);

	xy_coord mapPosToCoord(glm::vec3 pos);
	bool coordInBounds(xy_coord coord);
};