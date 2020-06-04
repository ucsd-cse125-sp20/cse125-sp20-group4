#pragma once

#include "glm/glm.hpp"

#include <list>
#include <queue>
#include <unordered_map>

#include "ObjectClasses/objects.h"

#define MAX_DIM 1024

class MapRep {
private:
	struct MapCoord {
		int x, z;
	};

	struct block {
		std::shared_ptr<Object> obj = nullptr;
		int pathCost;
		block();
		block(std::shared_ptr<Object>obj, int val);
	};

	block map[MAX_DIM][MAX_DIM];
	int height, width;
public:
	
	void addObject(std::shared_ptr<Object> object, glm::vec3 pos);
	void removeObject(glm::vec3 pos);
	bool containsObject(glm::vec3 pos);
	std::shared_ptr<Object> getObjectAtCoord(MapCoord pos);
	std::shared_ptr<Object> getObjectAtPos(glm::vec3 pos);

	std::list<glm::vec3> getPath(glm::vec3 start, glm::vec3 end);
	MapRep(int height = 100, int width = 100);

	MapCoord mapPosToCoord(glm::vec3 pos);
	glm::vec3 coordToMapPos(MapCoord pos);
	void GridifyMapPos(glm::vec3& pos);
	bool coordInBounds(MapCoord coord);
};
