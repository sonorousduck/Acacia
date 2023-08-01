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
			bool hasDelay = particleGroup->startDelay != std::chrono::microseconds::zero() && particleGroup->duration < particleGroup->startDelay;
			particleGroup->duration += elapsedTime;



			// This will be the case most of the time. I want the compiler to optimize to go down the path that it IS preallocated, rather than it not being preallocated
			if (particleGroup->preallocated) {}
			else
			{
				Preallocate(particleGroup);
			}

			if (particleGroup->maxDuration == std::chrono::microseconds::zero() || particleGroup->duration < particleGroup->maxDuration + particleGroup->startDelay && !hasDelay)
			{
				particleGroup->accumulatedTime += elapsedTime;


				while (particleGroup->accumulatedTime >= particleGroup->spawnRate)
				{
					// Rate over time is the number of new particles each update loop
					for (unsigned int i = 0; i < particleGroup->rateOverTime; i++)
					{
						int unusedParticle = firstUnusedParticle(particleGroup);
						// TODO: This will eventually be the game object, not the particle group that is passed
						respawnParticle(particleGroup->particles[unusedParticle], particleGroup, glm::vec2(1.0f));
					}
					particleGroup->accumulatedTime -= particleGroup->spawnRate;
				}
			}

			// If there is a start delay, we don't want to do any of the stuff after this point
			if (!hasDelay)
			{
				// TODO: Potentially, to avoid reallocation every time, this should be moved somewhere else.
				particleGroup->particlePositionSizeData.clear();
				particleGroup->particlePositionSizeData.resize(particleGroup->particles.size() * 4); // x, y, xSize, ySize

				particleGroup->particleColorData.clear();
				particleGroup->particleColorData.resize(particleGroup->particles.size() * 4); // r, g, b, a


				std::uint32_t particleCount = 0;

				for (unsigned int i = 0; i < particleGroup->particles.size(); i++)
				{
					Particle& particle = particleGroup->particles[i];

					// If lifetime is 0 microseconds, this means that it will always play
					if (particle.alive < particle.lifetime)
					{
						particle.alive += elapsedTime;

						// TODO: Eventually, make this a more complicated update time loop. That way, we can use animations inside of the particles

						particle.position += particle.currentSpeed * glm::vec2(elapsedTime.count() / 100000.0, elapsedTime.count() / 100000.0);

						float lerpValue = static_cast<float>(particle.alive.count()) / static_cast<float>(particle.lifetime.count());

						// Update size
						if (particle.lerpSize)
						{
							particle.currentSize = glm::mix(particle.startSize, particle.endSize, lerpValue);
						}

						if (particle.lerpAlpha)
						{
							particle.currentAlpha = std::lerp(particle.startAlpha, particle.endAlpha, lerpValue);
						}

						if (particle.lerpColor)
						{
							particle.currentColor.rgba = glm::mix(particle.startColor.rgba, particle.endColor.rgba, lerpValue);
						}

						particle.rotation += particle.rotationRate * elapsedTime.count() / 100000.0f;

						// Update the buffers with the new information
						particleGroup->particlePositionSizeData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 0] = particle.position.x;
						particleGroup->particlePositionSizeData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 1] = particle.position.y;
						particleGroup->particlePositionSizeData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 2] = particle.currentSize.x;
						particleGroup->particlePositionSizeData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 3] = particle.currentSize.y;

						particleGroup->particleColorData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 0] = particle.currentColor.r();
						particleGroup->particleColorData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 1] = particle.currentColor.g();
						particleGroup->particleColorData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 2] = particle.currentColor.b();
						particleGroup->particleColorData[4 * static_cast<std::vector<float, std::allocator<float>>::size_type>(particleCount) + 3] = particle.currentAlpha;

						particleCount++;
					}
				}

				particleGroup->particleCount = particleCount;

			}
		}
	}

	void ParticleSystem::Preallocate(components::ParticleGroup* particleGroup)
	{
		particleGroup->particles.reserve(particleGroup->getMaxParticles());
		while (particleGroup->particles.size() < particleGroup->getMaxParticles())
		{
			particleGroup->particles.push_back(Particle(particleGroup));
		}
		particleGroup->preallocated = true;
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
		particle.position = particleGroup->position + random + offset;

		particle.alive = std::chrono::microseconds::zero();
		particle.lifetime = particleGroup->maxLifetime;

	}
}