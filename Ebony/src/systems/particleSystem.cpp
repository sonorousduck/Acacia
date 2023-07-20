#include "particleSystem.hpp"


namespace systems
{
	void ParticleSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void ParticleSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto particleGroup = entity->getComponent<components::ParticleGroup>();

			// This will be the case most of the time. I want the compiler to optimize to go down the path that it IS preallocated, rather than it not being preallocated
			if (particleGroup->preallocated) {}
			else
			{
				Preallocate(particleGroup);
			}

			// Rate over time is the number of new particles each update loop
			for (unsigned int i = 0; i < particleGroup->rateOverTime; i++)
			{
				int unusedParticle = firstUnusedParticle(particleGroup);
				// TODO: This will eventually be the game object, not the particle group that is passed
				respawnParticle(particleGroup->particles[unusedParticle], particleGroup, glm::vec2(0.0f));
			}

			for (unsigned int i = 0; i < particleGroup->particles.size(); i++)
			{
				Particle& particle = particleGroup->particles[i];
				
				if (particle.alive < particle.lifetime)
				{
					particle.alive += elapsedTime;


				}
			}


		}
	}

	void ParticleSystem::Preallocate(components::ParticleGroup* particleGroup)
	{
		while (particleGroup->particles.size() < particleGroup->maxParticles)
		{
			particleGroup->particles.push_back(Particle());
		}
	}

	int ParticleSystem::firstUnusedParticle(components::ParticleGroup* particleGroup)
	{
		for (unsigned int i = particleGroup->unusedParticleIndex; i < particleGroup->maxParticles; i++)
		{
			if (particleGroup->particles[i].alive >= particleGroup->particles[i].lifetime)
			{
				particleGroup->unusedParticleIndex = i;
				return i;
			}
		}

		// Otherwise, do a linear search
		for (unsigned int i = 0; i < particleGroup->unusedParticleIndex; i++)
		{
			if (particleGroup->particles[i].alive >= particleGroup->particles[i].lifetime)
			{
				particleGroup->unusedParticleIndex = i;
				return i;
			}
		}

		// This should hardly ever call. If this calls, more particles should be removed (Especially because that means we just linearly searched
		// through all the particles in creation, which could get to be quite a lot
		particleGroup->unusedParticleIndex = 0;
		return 0;
	}

	// This should also be modified to take into account the things that the particle group defines, such as
	// its shape, emission velocity, etc.
	void ParticleSystem::respawnParticle(Particle& particle, components::ParticleGroup* particleGroup, glm::vec2 offset)
	{
		float random = ((rand() % 100) - 50) / 10.0f;
		float rColor = 0.5f + ((rand() % 100) / 100.0f);
		particle.position = particleGroup->position + random + offset;
		particle.startColor = Ebony::Color(rColor, rColor, rColor);
		particle.endColor = Ebony::Color(rColor, rColor, rColor);
		particle.currentColor = Ebony::Color(rColor, rColor, rColor);

		particle.alive = std::chrono::microseconds::zero();
		particle.lifetime = std::chrono::microseconds(1000);
		particle.velocity = particleGroup->velocity * 1.2f;
	}
}