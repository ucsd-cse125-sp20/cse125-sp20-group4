#pragma once

#include "glm/glm.hpp"

#include <list>
#include <queue>
#include <unordered_map>

#include "ObjectClasses/objects.h"


class MapRep {
public:
	struct xy_coord {
		int x, z;

		xy_coord() {
			x = 0;
			z = 0;
		};

		xy_coord(const xy_coord& other) {
			x = other.x;
			z = other.z;
		};

		xy_coord(int xIn, int zIn) {
			x = xIn;
			z = zIn;
		};


		bool operator== (const xy_coord& c1) const {
			return (x == c1.x && z == c1.z);
		};
		
		bool operator!= (const xy_coord& c1) const {
			return (x != c1.x || z != c1.z);
		};

		bool operator< (const xy_coord& c1) const {
			return (x + z < c1.x + c1.z);
		};

		size_t operator() (const xy_coord& c1) const {
			return (c1.x + 10 * c1.z);
		};

		//bool operator()(const xy_coord& c1) const {}

	};

	struct XyHash {
		inline size_t operator()(const MapRep::xy_coord& c1) const{
			size_t value = (c1.x + 10 * c1.z);

			return value;
		}
	};

	struct block {
		std::shared_ptr<Object> obj = nullptr;
		int value;
		block();
		block(std::shared_ptr<Object>obj, int val);
	};

	
	void addObject(std::shared_ptr<Object> object, glm::vec3 pos);
	void removeObject(glm::vec3 pos);
	bool containsObject(glm::vec3 pos);
	std::shared_ptr<Object> getObjectAtCoord(xy_coord pos);
	std::shared_ptr<Object> getObjectAtPos(glm::vec3 pos);

	std::vector<glm::vec3> getPath(glm::vec3 start, glm::vec3 end);
	MapRep(int height = 100, int width = 100);

	xy_coord mapPosToCoord(glm::vec3 pos);
	glm::vec3 coordToMapPos(xy_coord pos);
	void GridifyMapPos(glm::vec3& pos);
	bool coordInBounds(xy_coord coord);

private:
	block map[200][200];
	int height, width;

};


