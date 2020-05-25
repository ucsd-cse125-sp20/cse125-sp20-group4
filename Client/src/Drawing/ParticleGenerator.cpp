#pragma warning(disable:4201)

#include "drawing/ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(const Shader& shader, Texture* texture, unsigned int amount) : shader(shader), texture(texture), amount(amount) {
	this->init();
}

void ParticleGenerator::Update(float dt, Entity* entity, unsigned int newParticles, glm::vec2 offset) {
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = this->firstUnusedParticle();
        this->respawnParticle(this->particles[unusedParticle], entity, offset);
    }

    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f)
        {
            p.Position -= p.Velocity * dt;
            p.Color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::Draw(const glm::mat4x4& P, const glm::mat4x4& V) {

    // zero out view matrix rotation to billboard particles
    /*glm::mat4x4 view = glm::mat4x4(1.0f);
    view[3] = V[3];
    glm::mat4x4 pv = P * view;*/
    glm::mat4x4 pv = P * V;

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "pv"), 1, GL_FALSE, &pv[0][0]);

    for (Particle particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            glUniform2fv(glGetUniformLocation(shader, "offset"), 1, glm::value_ptr(particle.Position));
            glUniform4fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(particle.Color));
            texture->Bind();
            glBindVertexArray(this->VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
    unsigned int VBO;
    float particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    for (unsigned int i = 0; i < this->amount; ++i) {
        this->particles.push_back(Particle());
    }
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle() {

    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, Entity* entity, glm::vec2 offset) {
    float randomX = ((rand() % 100) - 50) / 100.0f;
    float randomY = ((rand() % 100) - 50) / 100.0f;
    float rColor = 0.7f + ((rand() % 100) / 100.0f);
    particle.Position = glm::vec2(entity->getPosition().x, entity->getPosition().y) + glm::vec2(randomX, randomY) + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;
    float randomVX = ((rand() % 100) - 50) / 100.0f;
    float randomVY = ((rand() % 100) - 50) / 100.0f;
    particle.Velocity = glm::vec2(entity->getVelocity().x, entity->getVelocity().y) * 0.1f + glm::vec2(randomVX, randomVY);
}