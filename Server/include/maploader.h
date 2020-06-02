#pragma once
#include <glm/vec3.hpp>

#include <string>
#include <vector>
#include <fstream>

#include "ObjectClasses/objects.h"
#include "gamestate.h"

class MapLoader {
public:
	static void LoadMap(std::string file, GameState* gs);
};