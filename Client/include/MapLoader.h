#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <glm/vec3.hpp>

#include <string>
#include <vector>
#include <fstream>

#include "state/Entity.h"
#include "drawing/model/RectangularCuboid.h"
#include "drawing/model/LoadedModel.h"
#include "drawing/Shaders.h"

class MapLoader {

private:
	std::vector<Entity *> entities;

public:
    MapLoader(const std::string file);

	std::vector<Entity *> getEntities();
};

#endif // !RECTANGULAR_CUBOID_H
