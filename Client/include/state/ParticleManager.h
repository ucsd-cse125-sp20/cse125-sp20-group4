#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include <vector>
#include <unordered_map>

#include "drawing/ParticleGenerator.h"
#include "drawing/TextureManager.h"

class ParticleManager {
private:
	std::vector<Entity *> entities;
	std::unordered_map<Entity *, ParticleGenerator *> trails;
	std::unordered_map<Entity *, ParticleGenerator *> explosions;
	TextureManager* tmanager;
public:
	ParticleManager(TextureManager* tmanager);
	void addTrail(Entity * entity);
	void addExplosion(Entity * entity);
	void update();
	void draw(const glm::mat4x4& view, const glm::vec3& cameraPos);
};
#endif
