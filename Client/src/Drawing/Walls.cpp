#include "drawing/Walls.h"

Walls::Walls(std::vector<Texture *> textures, glm::vec3& closeLeft, glm::vec3& farRight) {

	walls[0] = new Plane(textures[0], closeLeft, glm::vec3(farRight.x, closeLeft.y, farRight.z));
	walls[1] = new Plane(textures[1], glm::vec3(farRight.x, farRight.y, closeLeft.z), glm::vec3(closeLeft.x, farRight.y, farRight.z));
	walls[2] = new Plane(textures[2], closeLeft, glm::vec3(closeLeft.x, farRight.y, farRight.z));
	walls[3] = new Plane(textures[3], glm::vec3(farRight.x, closeLeft.y, farRight.z), glm::vec3(farRight.x, farRight.y, closeLeft.z));
	walls[4] = new Plane(textures[4], glm::vec3(closeLeft.x, closeLeft.y, farRight.z), farRight);
	walls[5] = new Plane(textures[5], glm::vec3(farRight.x, closeLeft.y, closeLeft.z), glm::vec3(closeLeft.x, farRight.y, closeLeft.z));

}

void Walls::Draw(const glm::mat4x4& view) {
	for (int i = 0; i < 6; i++) {
		walls[i]->draw(glm::mat4x4(1.0f), view);
	}
}