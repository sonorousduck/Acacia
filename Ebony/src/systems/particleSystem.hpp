#pragma once
#include "system.hpp"
#include "../components/particle.hpp"

namespace systems
{
	class ParticleSystem : public System
	{
		ParticleSystem() : System({ ctti::unnamed_type_id<components::ParticleGroup>() })
		{
		}

		virtual void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
		void Preallocate(components::ParticleGroup* particleGroup);
		int firstUnusedParticle(components::ParticleGroup* particleGroup);
		void respawnParticle(Particle& particle, components::ParticleGroup* particleGroup, glm::vec2 offset);
	};


}