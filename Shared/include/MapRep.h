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
	
	void addObject(std::shared_ptr<Object> object, glm::vec3 pos);
	void removeObject(glm::vec3 pos);
	bool containsObject(glm::vec3 pos);

	std::vector<glm::vec3> getPath(glm::vec3 start, glm::vec3 end);
	MapRep(int height = 100, int width = 100);
	xy_coord mapPosToCoord(glm::vec3 pos);
	glm::vec3 coordToMapPos(xy_coord pos);
	bool coordInBounds(xy_coord coord);

private:
	int map[200][200];
	int height, width;

};