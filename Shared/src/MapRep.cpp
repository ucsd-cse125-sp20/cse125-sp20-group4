#include "MapRep.h"
#include "logger.h"
MapRep::block::block() {
	this->obj = nullptr;
	this->pathCost = 0;
}

MapRep::block::block(std::shared_ptr<Object> obj, int val) {
	this->obj = obj;
	this->pathCost = val;
}

MapRep::MapRep(int height, int width) :
	height(height > MAX_DIM ? MAX_DIM : height),
	width(width > MAX_DIM ? MAX_DIM : width) {
	// set the map to be empty at the beginning
	for (int r = 0; r < this->height; r++) {
		for (int c = 0; c < this->width; c++) {
			map[r][c] = block(nullptr, 1);
		}
	}
}

void MapRep::addObject(std::shared_ptr<Object> object, glm::vec3 pos)
{
	auto log = getLogger("MapRep");
	MapCoord coord = mapPosToCoord(pos);
	if (!coordInBounds(coord)) return;
	// Barricade = 5 in case we implement weighted pathing
	if (std::dynamic_pointer_cast<Barricade>(object) != nullptr) {
		map[coord.x][coord.z] = block(object, 5);
	}
	// Any other object = 100
	else {
		map[coord.x][coord.z] = block(object, 100);
	}

	log->trace("adding object to map");
}

void MapRep::removeObject(glm::vec3 pos)
{
	MapCoord coord = mapPosToCoord(pos);
	if (!coordInBounds(coord)) return;
	if (map[coord.x][coord.z].obj != nullptr) {
		map[coord.x][coord.z].obj = nullptr;
		map[coord.x][coord.z].pathCost = 0;
	}
}

bool MapRep::containsObject(glm::vec3 pos)
{
	MapCoord coord = mapPosToCoord(pos);
	if (!coordInBounds(coord))
		return true;
	return map[coord.x][coord.z].pathCost != 0;
}

std::shared_ptr<Object> MapRep::getObjectAtCoord(MapCoord pos) {
	if (!coordInBounds(pos))
		return nullptr;
	return map[pos.x][pos.z].obj;
}

std::shared_ptr<Object> MapRep::getObjectAtPos(glm::vec3 pos) {
	auto log = getLogger("MapRep");
	MapCoord coords = mapPosToCoord(pos);
	log->trace("Getting object at position ({},{},{}), coords ({},0,{})", pos.x, pos.y, pos.z,coords.x,coords.z);
	if (!coordInBounds(coords))
		return nullptr;
	return map[coords.x][coords.z].obj;
}

std::list<glm::vec3> MapRep::getPath(glm::vec3 start, glm::vec3 end)
{
	struct Node {
		bool visited;
		MapCoord prev;
	};
	MapCoord start_coord = mapPosToCoord(start);
	MapCoord end_coord = mapPosToCoord(end);

	// if invalid start and end, return empty
	if (!coordInBounds(start_coord) || !coordInBounds(end_coord)) {
		return std::list<glm::vec3>();
	}

	// use flattened 2d array for speed, no need for hash funcs, etc
	Node* nodes = new Node[(long long) height * (long long) width]();
	auto flatten = [=](MapCoord coord) {
		return coord.x * width + coord.z;
	};

	std::queue<MapCoord> queue;

	queue.push(start_coord);
	nodes[start_coord.x * width + start_coord.z].visited = true;
	// iterate until end coord is found
	while (!queue.empty() && !nodes[flatten(end_coord)].visited) {
		MapCoord coord = queue.front();
		queue.pop();

		// check x +- 1
		for (int i = -1; i <= 1; i += 2) {
			MapCoord next = { coord.x + i, coord.z };
			if (
				!nodes[flatten(next)].visited &&
				coordInBounds(next) &&
				map[next.x][next.z].pathCost == 1
			) {
				queue.push(next);
				nodes[flatten(next)].visited = true;
				nodes[flatten(next)].prev = coord;
			}
		}

		// check z +- 1
		for (int i = -1; i <= 1; i += 2) {
			MapCoord next = { coord.x, coord.z + i };
			if (
				!nodes[flatten(next)].visited &&
				coordInBounds(next) &&
				map[next.x][next.z].pathCost == 1
			) {
				queue.push(next);
				nodes[flatten(next)].visited = true;
				nodes[flatten(next)].prev = coord;
			}
		}
	}

	std::list<glm::vec3> pathlist;
	// rebuild path if the end is found
	if (nodes[end_coord.x * width + end_coord.z].visited) {
		MapCoord backtrack = end_coord;
		while (
			nodes[flatten(backtrack)].prev.x != start_coord.x ||
			nodes[flatten(backtrack)].prev.z != start_coord.z
		) {
			pathlist.push_front(glm::vec3(backtrack.x, 0, backtrack.z));
			backtrack = nodes[flatten(backtrack)].prev;
		}
	}
	
	delete nodes;
	return pathlist;
}

MapRep::MapCoord MapRep::mapPosToCoord(glm::vec3 pos)
{
	auto log = getLogger("MapRep");
	MapCoord coord;
	coord.x = int(round(pos.x));
	coord.z = int(round(pos.z));
	log->trace("Converting to coords from pos ({},X,{}) to ({},X,{})", pos.x, pos.z, coord.x, coord.z);
	return coord;
}
glm::vec3 MapRep::coordToMapPos(MapCoord pos) {
	return glm::vec3(pos.x, 0.0, pos.z);
}

bool MapRep::coordInBounds(MapCoord coord) {
	return coord.x >= 0 && coord.x < height && coord.z >= 0 && coord.z < width;
}


void MapRep::GridifyMapPos(glm::vec3& pos) {
	pos.x = round(pos.x);
	pos.y = 0.0f;
	pos.z = round(pos.z);
}