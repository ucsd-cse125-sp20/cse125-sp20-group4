#pragma warning(disable:4201)

#include "drawing/ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(const Shader& shader, Texture* texture, unsigned int amount, float scale) : shader(shader), texture(texture), amount(amount), scale(scale) {
	init();
}

ParticleGenerator::~ParticleGenerator() {

    /* Delete previously generated buffers */
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, VBO);

}

void ParticleGenerator::Update(float dt, Entity* entity, unsigned int newParticles, glm::vec3 offset, glm::vec3 acc) {
    for (unsigned int i = 0; i < newParticles; ++i)
    {
        int unusedParticle = firstUnusedParticle();
        respawnParticle(particles[unusedParticle], entity, offset);
        positions[unusedParticle] = particles[unusedParticle].Position;
        colors[unusedParticle] = particles[unusedParticle].Color;
    }

    for (unsigned int i = 0; i < amount; ++i)
    {
        Particle& p = particles[i];
        p.Life -= dt;
        if (p.Life > 0.0f)
        {
            p.Position += p.Velocity * dt + acc * dt * dt / 2.0f;
            positions[i] = p.Position;
            p.Velocity += acc * dt;
            p.Color.a = p.Life;
            colors[i] = p.Color;
        }
    }

    life += dt;
}

void ParticleGenerator::Draw(const glm::mat4x4& view, const glm::vec3& cameraPos) {

    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "pv"), 1, GL_FALSE, &view[0][0]);
    glUniform1f(glGetUniformLocation(shader, "scale"), scale);
    glUniform3fv(glGetUniformLocation(shader, "cameraPos"), 1, glm::value_ptr(cameraPos));

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * colors.size(), colors.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    texture->Bind();
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_POINT_SPRITE);
    glDrawArrays(GL_POINTS, 0, (GLsizei)positions.size());
    glBindVertexArray(0);

    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Clear() {
    for (unsigned int i = 0; i < amount; i++) {
        particles[i] = Particle();
        positions[i] = particles[i].Position;
        colors[i] = particles[i].Color;
    }

    life = 0.0f;
}

float ParticleGenerator::getLife() {
    return life;
}

void ParticleGenerator::init() {

    for (unsigned int i = 0; i < amount; ++i) {
        particles.push_back(Particle());
        positions.push_back(particles[i].Position);
        colors.push_back(particles[i].Color);
    }

    life = 0.0f;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(2, VBO);
   
}

unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle() {

    for (unsigned int i = lastUsedParticle; i < amount; ++i) {
        if (particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }

    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, Entity* entity, glm::vec3 offset) {
    float rColor = 0.7f + ((rand() % 100) / 100.0f);
    particle.Position = entity->getPosition() + offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 1.0f;

    float velocity = ((rand() % 100) + 1) / 25.0f;
    float angle = (float)(rand() % 360);
    float vx = cos(angle) * velocity;
    float vy = ((rand() % 50) + 10) / 10.0f;
    float vz = sin(angle) * velocity;
    particle.Velocity = glm::vec3(vx, vy, vz) + entity->getVelocity() * 0.8f;
}