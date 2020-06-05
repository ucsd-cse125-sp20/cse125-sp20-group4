#pragma once

#include "drawing/model/Plane.h"

class Walls {
private:
	// 0 = floor
	// 1 = ceiling
	// 2 = left wall
	// 3 = right wall
	// 4 = forward wall
	// 5 = behind wall
	Model * walls[6];
public:
	Walls(std::vector<Texture *> textures, glm::vec3& closeLeft, glm::vec3& farRight);

	void Draw(const glm::mat4x4& view);
};
