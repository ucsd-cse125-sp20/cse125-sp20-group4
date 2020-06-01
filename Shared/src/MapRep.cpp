#include "MapRep.h"

MapRep::MapRep(int height, int width) : height(height), width(width) {
	// set the map to be empty at the beginning
	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			map[r][c] = 0;
		}
	}
}

void MapRep::addObject(std::shared_ptr<Object> object, glm::vec3 pos)
{
	xy_coord coord = mapPosToCoord(pos);
	if (!coordInBounds(coord)) return;
	// Barricade = 5 in case we implement weighted pathing
	if (std::dynamic_pointer_cast<Barricade>(object) != nullptr) {
		map[coord.x][coord.z] = 5;
	}
	// Any other object = 100
	else {
		map[coord.x][coord.z] = 100;
	}
}

void MapRep::removeObject(glm::vec3 pos)
{
	xy_coord coord = mapPosToCoord(pos);
	if (!coordInBounds(coord)) return;
	if (map[coord.x][coord.z] == 5)
		map[coord.x][coord.z] = 0;
}

bool MapRep::containsObject(glm::vec3 pos)
{
	xy_coord coord = mapPosToCoord(pos);
	if (!coordInBounds(coord))
		return true;
	return map[coord.x][coord.z] != 0;
}

std::vector<glm::vec3> MapRep::getPath(glm::vec3 /*start*/, glm::vec3 /*end*/)
{
	/*
	xy_coord start_coord = mapPosToCoord(start);
	xy_coord end_coord = mapPosToCoord(end);
	if (!coordInBounds(start_coord) || !coordInBounds(end_coord))
		return std::vector<glm::vec3>();
	std::unordered_map<xy_coord, xy_coord> visited;
	std::queue<xy_coord> queue;

	queue.push(start_coord);
	visited.emplace(start_coord, NULL);
	while (!queue.empty()) {
		xy_coord coord = queue.pop();

		// rebuild path when the end is found
		if (coord.x == end_coord.x && coord.z == end_coord.z) {
			std::list<glm::vec3> solution;
			while (visited[coord] != &start_coord) {
				solution.push_front(glm::vec3(coord->x, 0, coord->z));
				coord = visited[coord];
			}
			return solution;
		}

		xy_coord adj_coord(coord.x - 1, coord.z, &coord);
		if (coordInBounds(adj_coord) && map[adj_coord.x][adj_coord.z] == 1 &&) {
			queue.push(adj_coord);
			visited.emplace(adj_coord, coord);
		}
		xy_coord adj_coord(coord.x + 1, coord.z, &coord);
		if (coordInBounds(adj_coord) && map[adj_coord.x][adj_coord.z] == 1) {
			queue.push(adj_coord);
			visited.emplace(adj_coord, coord);
		}
		xy_coord adj_coord(coord.x, coord.z - 1, &coord);
		if (coordInBounds(adj_coord) && map[adj_coord.x][adj_coord.z] == 1) {
			queue.push(adj_coord);
			visited.emplace(adj_coord, coord);
		}
		xy_coord adj_coord(coord.x, coord.z + 1, &coord);
		if (coordInBounds(adj_coord) && map[adj_coord.x][adj_coord.z] == 1) {
			queue.push(adj_coord);
			visited.emplace(adj_coord, coord);
		}
	}
	*/

	return std::vector<glm::vec3>();
}

MapRep::xy_coord MapRep::mapPosToCoord(glm::vec3 pos)
{
	xy_coord coord;
	coord.x = (int)(pos.x + 0.5f);
	coord.z = (int)(pos.z + 0.5f);
	return xy_coord();
}
glm::vec3 MapRep::coordToMapPos(xy_coord pos) {
	return glm::vec3(pos.x, 0.0, pos.z);
}

bool MapRep::coordInBounds(xy_coord coord) {
	return coord.x >= 0 && coord.x < height && coord.z >= 0 && coord.z < width;
}
