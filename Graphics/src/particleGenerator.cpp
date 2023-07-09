#include "particleGenerator.hpp"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
	: shader(shader), texture(texture), amount(amount)
{
	this->init();
}

void ParticleGenerator::Update(float dt, unsigned int newParticles, glm::vec2 offset)
{
	// Add new particles
	for (unsigned int i = 0; i < newParticles; i++)
	{
		int unusedParticle = this->firstUnusedParticle();
		this->respawnParticle(this->particles[unusedParticle], offset);
	}

	// Update all particles
	for (unsigned int i = 0; i < this->amount; i++)
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

// Render
void ParticleGenerator::Draw()
{
	// Use additive blending to give it a 'glow' effect
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	this->shader.use();
	for (Particle particle : this->particles)
	{
		if (particle.Life > 0.0f)
		{
			this->shader.setVec2("offset", particle.Position);
			this->shader.setVec4("color", particle.Color);

			this->texture.Bind();
			glBindVertexArray(this->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
	// Set up mesh and attributes
	unsigned int VBO = 0;
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

	this->particles.reserve(amount);

	for (unsigned int i = 0; i < this->amount; i++)
	{
		this->particles.push_back(Particle());
	}
}

unsigned int lastUsedParticle = 0;;
unsigned int ParticleGenerator::firstUnusedParticle()
{
	// First search from last used particle
	for (unsigned int i = lastUsedParticle; i < this->amount; i++)
	{
		if (this->particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	// Otherwise, do a linear search
	for (unsigned int i = 0; i < lastUsedParticle; i++)
	{
		if (this->particles[i].Life <= 0.0f)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	// All particles are taken, override the first one (if this case is hit often, more particles should be reserved)
	lastUsedParticle = 0;
	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, glm::vec2 offset)
{
	float random = ((rand() % 100) - 50) / 10.0f;
	float rColor = 0.5f + ((rand() % 100) / 100.0f);

	particle.Position = random + offset + glm::vec2(20.0, 20.0); // This should have some sort of object position added to it (so you can specify where the generator should be at)
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity = glm::vec2(1.0f, 1.0f) * 0.1f; // This should also have the object velocity
}