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
				respawnParticle(particleGroup->particles[unusedParticle], particleGroup, glm::vec2(1.0f));
			}
			
			// TODO: Potentially, to avoid reallocation every time, this should be moved somewhere else.
			particleGroup->particlePositionSizeData.clear();
			particleGroup->particlePositionSizeData.resize(particleGroup->particles.size() * 16); // x, y, xSize, ySize

			particleGroup->particleColorData.clear();
			particleGroup->particleColorData.resize(particleGroup->particles.size() * 4); // r, g, b, a
			

			std::uint32_t particleCount = 0;

			for (unsigned int i = 0; i < particleGroup->particles.size(); i++)
			{
				Particle& particle = particleGroup->particles[i];
				
				// If lifetime is 0 microseconds, this means that it will always play
				if (particle.alive < particle.lifetime)
				{
					// TODO: Eventually, make this a more complicated update time loop. That way, we can use animations inside of the particles
					particle.alive += elapsedTime;

					particle.position += particle.velocity * glm::vec2(elapsedTime.count() / 100000.0, elapsedTime.count() / 100000.0) ;

					float lerpValue = static_cast<float>(particle.alive.count()) / static_cast<float>(particle.lifetime.count());

					// Update size
					if (particle.startSize != particle.endSize)
					{
						particle.currentSize.x = std::lerp(particle.startSize.x, particle.endSize.x, lerpValue);
						particle.currentSize.y = std::lerp(particle.startSize.y, particle.endSize.y, lerpValue);
					}

					if (particle.startAlpha != particle.endAlpha)
					{
						particle.currentAlpha = std::lerp(particle.startAlpha, particle.endAlpha, lerpValue);
					}

					if (particle.startColor != particle.endColor)
					{
						particle.currentColor.setR(std::lerp(particle.startColor.r(), particle.endColor.r(), lerpValue));
						particle.currentColor.setG(std::lerp(particle.startColor.g(), particle.endColor.g(), lerpValue));
						particle.currentColor.setB(std::lerp(particle.startColor.b(), particle.endColor.b(), lerpValue));
					}

					particle.rotation += particle.rotationRate * elapsedTime.count() / 100000.0f;

					// Update the buffers with the new information
					//glm::mat4 model = glm::mat4(1.0f);
					//// TODO: THE 0.0 SHOULD BE CHANGED TO THE DEPTH
					//model = glm::translate(model, glm::vec3(particle.position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

					//model = glm::translate(model, glm::vec3(0.5f * particle.currentSize.x, 0.5f * particle.currentSize.y, 0.0f)); // move origin of rotation to center of quad
					//model = glm::rotate(model, glm::radians(particle.rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
					//model = glm::translate(model, glm::vec3(-0.5f * particle.currentSize.x, -0.5f * particle.currentSize.y, 0.0f)); // move origin back
					//model = glm::scale(model, glm::vec3(particle.currentSize, 1.0f)); // last scale

					//particleGroup->particlePositionSizeData[16 * i]     = model[0].x;
					//particleGroup->particlePositionSizeData[16 * i + 1] = model[0].y;
					//particleGroup->particlePositionSizeData[16 * i + 2] = model[0].z;
					//particleGroup->particlePositionSizeData[16 * i + 3] = model[0].w;

					//particleGroup->particlePositionSizeData[16 * i + 4] = model[1].x;
					//particleGroup->particlePositionSizeData[16 * i + 5] = model[1].y;
					//particleGroup->particlePositionSizeData[16 * i + 6] = model[1].z;
					//particleGroup->particlePositionSizeData[16 * i + 7] = model[1].w;

					//particleGroup->particlePositionSizeData[16 * i + 8] = model[2].x;
					//particleGroup->particlePositionSizeData[16 * i + 9] = model[2].y;
					//particleGroup->particlePositionSizeData[16 * i + 10] = model[2].z;
					//particleGroup->particlePositionSizeData[16 * i + 11] = model[2].w;

					//particleGroup->particlePositionSizeData[16 * i + 12] = model[3].x;
					//particleGroup->particlePositionSizeData[16 * i + 13] = model[3].y;
					//particleGroup->particlePositionSizeData[16 * i + 14] = model[3].z;
					//particleGroup->particlePositionSizeData[16 * i + 15] = model[3].w;


					particleGroup->particlePositionSizeData[4 * i] = particle.position.x;
					particleGroup->particlePositionSizeData[4 * i + 1] = particle.position.y;
					particleGroup->particlePositionSizeData[4 * i + 2] = particle.currentSize.x;
					particleGroup->particlePositionSizeData[4 * i + 3] = particle.currentSize.y;

					particleGroup->particleColorData[4 * i] = particle.currentColor.r();
					particleGroup->particleColorData[4 * i + 1] = particle.currentColor.g();
					particleGroup->particleColorData[4 * i + 2] = particle.currentColor.b();
					particleGroup->particleColorData[4 * i + 3] = particle.currentAlpha;

					particleCount++;
				}
			}

			particleGroup->particleCount = particleCount;
		}
	}

	void ParticleSystem::Preallocate(components::ParticleGroup* particleGroup)
	{
		while (particleGroup->particles.size() < particleGroup->getMaxParticles())
		{
			particleGroup->particles.push_back(Particle(particleGroup->texture, std::chrono::microseconds::zero(), particleGroup->startSize, particleGroup->endSize, particleGroup->startAlpha, particleGroup->endAlpha));
		}
	}

	int ParticleSystem::firstUnusedParticle(components::ParticleGroup* particleGroup)
	{
		for (unsigned int i = particleGroup->unusedParticleIndex; i < particleGroup->getMaxParticles(); i++)
		{
			if (particleGroup->particles[i].alive >= particleGroup->particles[i].lifetime)
			{
				particleGroup->unusedParticleIndex = i;
				return i;
			}
		}

		// Otherwise, do a linear search
		for (int i = 0; i < particleGroup->unusedParticleIndex; i++)
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
		//particle.position = particleGroup->position;

		particle.startColor = Ebony::Color(rColor, rColor, rColor);
		particle.endColor = Ebony::Color(rColor, rColor, rColor);
		particle.currentColor = Ebony::Color(rColor, rColor, rColor);

		particle.alive = std::chrono::microseconds::zero();
		particle.lifetime = particleGroup->maxLifetime;
		particle.velocity = particleGroup->velocity;

		if (rand() % 2 < 1)
		{
			particle.velocity.x = -particle.velocity.x;
		}
		if (rand() % 2 < 1)
		{
			particle.velocity.y = -particle.velocity.y;
		}


	}
}