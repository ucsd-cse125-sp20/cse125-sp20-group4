#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "drawing/Shaders.h"
#include "state/Entity.h"
#include "drawing/Texture.h"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(glm::vec2(0.0f)), Velocity(glm::vec2(0.0f)), Color(glm::vec4(1.0f)), Life(0.0f) { }
};

class ParticleGenerator
{
public:
    ParticleGenerator(const Shader & shader, Texture* texture, unsigned int amount);

    void Update(float dt, Entity* entity, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));
    void Draw(const glm::mat4x4& P, const glm::mat4x4& V);
private:
    std::vector<Particle> particles;
    const Shader& shader;
    Texture* texture;
    unsigned int amount;
    unsigned int VAO;
    
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, Entity* entity, glm::vec2 offset = glm::vec2(0.0f));
};

#endif
