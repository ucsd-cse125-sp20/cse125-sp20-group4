#pragma warning(disable:4201)

#include "state/ParticleManager.h"

ParticleManager::ParticleManager(TextureManager * tmanager) : tmanager(tmanager) {}

void ParticleManager::addTrail(Entity* entity)
{
	entities.push_back(entity);
	trails[entity] = new ParticleGenerator(Shaders::particle(), tmanager->get("smoke"), 1000, 1);
}

void ParticleManager::addExplosion(Entity* entity)
{
	entities.push_back(entity);
	explosions[entity] = new ParticleGenerator(Shaders::particle(), tmanager->get("smoke"), 1000, 1);
	explosions[entity]->Update(0.001f, entity, 1000, glm::vec3(0.0f));
}

void ParticleManager::update()
{
	auto it = entities.begin();
	while(it != entities.end()) {
		if (trails.find(*it) != trails.end()) {
			if ((*it)->getVelocity() != glm::vec3(0.0)) {
				trails[*it]->Update(0.01f, *it, 10, glm::vec3(0.0f,-0.5f,0.0f), glm::vec3(0.0f, -9.8f, 0.0f));
			}
			else {
				trails[*it]->Clear();
			}
			it++;
		}
		else {
			if (explosions[*it]->getLife() < 1.0f) {
				explosions[*it]->Update(0.01f, *it, 0, glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
				it++;
			}
			else {
				explosions.erase(*it);
				it = entities.erase(it);
			}
		}

	}
}

void ParticleManager::draw(const glm::mat4x4& view, const glm::vec3& cameraPos)
{
	for (auto it = trails.begin(); it != trails.end(); it++) {
		if (it->first->getVelocity() != glm::vec3(0.0)) {
			it->second->Draw(view, cameraPos);
		}
	}

	for (auto it = explosions.begin(); it != explosions.end(); it++) {
		it->second->Draw(view, cameraPos);
	}
}