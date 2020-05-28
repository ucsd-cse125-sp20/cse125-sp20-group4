#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <cmath>

#include "drawing/Shaders.h"
#include "state/Entity.h"
#include "drawing/Texture.h"

struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(glm::vec3(0.0f)), Velocity(glm::vec3(0.0f)), Color(glm::vec4(1.0f)), Life(0.0f) { }
};

class ParticleGenerator
{
public:
    ParticleGenerator(const Shader & shader, Texture* texture, unsigned int amount, float scale);
    ~ParticleGenerator();

    void Update(float dt, Entity* entity, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f));
    void Draw(const glm::mat4x4& view, const glm::vec3& cameraPos);
private:
    std::vector<Particle> particles;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec4> colors;
    const Shader& shader;
    Texture* texture;
    unsigned int amount;
    float scale;
    unsigned int VAO;
    unsigned int VBO[2];

    glm::vec3 acc = glm::vec3(0.0f, -9.8f, 0.0f);
    //glm::vec3 acc = glm::vec3(0.0f, -10.0f, 0.0f);

    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle& particle, Entity* entity, glm::vec3 offset = glm::vec3(0.0f));
};

#endif
