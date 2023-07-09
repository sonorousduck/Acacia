#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "texture.hpp"

struct Particle {
    glm::vec2 Position, Velocity;
    glm::vec4 Color;
    float Life;

    Particle()
        : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};


// Particle Generator acts as a container for rendering a large number of particles
// by repeatedly spawning, updating and killing the particles after a given amount of time
class ParticleGenerator
{
public:
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
    void Update(float dt, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();

private:
    std::vector<Particle> particles;
    unsigned int amount;

    Shader shader;
    Texture2D texture;
    unsigned int VAO;

    void init();

    unsigned int firstUnusedParticle();

    void respawnParticle(Particle& particle, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};