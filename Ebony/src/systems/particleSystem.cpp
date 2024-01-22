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
			if (!entity->isEnabled()) return;


			auto particleGroup = entity->getComponent<components::ParticleGroup>();
			auto transform = entity->getComponent<components::Transform>();
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
						respawnParticle(particleGroup->particles[unusedParticle], particleGroup->minAngle, particleGroup->maxAngle, particleGroup, transform);
					}
					particleGroup->accumulatedTime -= particleGroup->spawnRate;
				}
			}

			// If there is a start delay, we don't want to do any of the stuff after this point
			if (!hasDelay)
			{
				// TODO: Potentially, to avoid reallocation every time, this should be moved somewhere else.
				particleGroup->particlePositionSizeData.clear();
				particleGroup->particlePositionSizeData.resize(particleGroup->particles.size() * 4 * sizeof(float)); // x, y, xSize, ySize

				particleGroup->particleColorData.clear();
				particleGroup->particleColorData.resize(particleGroup->particles.size() * 4 * sizeof(float)); // r, g, b, a


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
		particleGroup->preallocated = true;
		particleGroup->particles.reserve(particleGroup->getMaxParticles());
		while (particleGroup->particles.size() < particleGroup->getMaxParticles())
		{
			particleGroup->particles.push_back(Particle(particleGroup));
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
	void ParticleSystem::respawnParticle(Particle& particle, float lowerBound, float upperBound, components::ParticleGroup* particleGroup, components::Transform* transform)
	{
		if (particleGroup->volume)
		{
			glm::vec2 random = particleGroup->random_double_vec2() * particleGroup->emissionArea;
			particle.position = transform->position + random;
		}
		else
		{
			glm::vec2 random = particleGroup->random_double_vec2();

			// We only want to spawn on the outskirts of the emission area
			particle.position = particleGroup->emissionArea * random - (particleGroup->emissionArea / 2.0f) + transform->position;
		}
		

		// If particleGroup arc angle is not 0, generate a random vector within the given arc angle
		if (lowerBound != upperBound)
		{
			double randomAngle = ((particleGroup->maxAngle - particleGroup->minAngle) * particleGroup->random_double() + particleGroup->minAngle) * std::numbers::pi / 180.0;
			glm::vec2 directionVector = glm::vec2(cos(randomAngle), sin(randomAngle));

			particle.startSpeed = directionVector * particleGroup->startMagnitude;
			particle.endSpeed = directionVector * particleGroup->endMagnitude;
			particle.currentSpeed = particle.startSpeed;
		}

		//particle.velocity = 

		particle.alive = std::chrono::microseconds::zero();
		particle.lifetime = particleGroup->maxLifetime;
	}
}