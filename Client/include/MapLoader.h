#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <glm/vec3.hpp>

#include <string>
#include <vector>
#include <fstream>

class MapLoader {

private:
	std::vector<std::string> modelFiles;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> directions;
	std::vector<float> scales;

public:
    MapLoader(const std::string file);

	std::vector<std::string> getModelFiles();
	std::vector<glm::vec3> getPositions();
	std::vector<glm::vec3> getDirections();
	std::vector<float> getScales();
};

#endif // !RECTANGULAR_CUBOID_H
