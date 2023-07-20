#pragma once

#include <glm/glm.hpp>
#include <chrono>

#include "component.hpp"
#include <colors.hpp>
#include <texture.hpp>


struct Particle
{
	Particle() {};
	Particle(std::chrono::microseconds lifetime, std::chrono::microseconds alive) : lifetime(lifetime), alive(alive) {};
	Particle(std::chrono::microseconds lifetime, std::chrono::microseconds alive, glm::vec2 position, glm::vec2 direction, glm::vec2 velocity) : 
		lifetime(lifetime), alive(alive), position(position), direction(direction), velocity(velocity) {};

	// Allows for fade in/fade out, etc.
	float startAlpha{ 1.0f };
	float endAlpha{ 1.0f };
	float currentAlpha{ 1.0f };

	// Allows control for the sizing of your particle through its life
	float startSize{ 0.0f };
	float endSize{ 0.0f };
	float currentSize{ 0.0f };

	glm::vec2 position{ 0.0f };
	glm::vec2 direction{ 0.0f };
	glm::vec2 velocity{ 0.0f };

	float rotationRate{ 0.0f };
	float rotation{ 0.0f };

	// Color over time
	Ebony::Color startColor = Ebony::Colors::White;
	Ebony::Color endColor = Ebony::Colors::White;
	Ebony::Color currentColor = Ebony::Colors::White;

	// Maximum lifetime allotted to the particle
	std::chrono::microseconds lifetime{ 0 };
	// How long the particle has been alive for
	std::chrono::microseconds alive{ 0 };

	// Particle texture
	Texture2D texture;
};
namespace components
{
	class ParticleGroup : public PolymorphicComparable<Component, ParticleGroup>
	{
	public:
		// For now, while there isn't a proper GameObject, the particle group will have a position
		glm::vec2 position{ 0.0f };

		// This defines the base velocity that each particle will be generated around
		glm::vec2 velocity{ 0.0f };

		// Tracks how long the particle group has been generating particles
		std::chrono::microseconds duration{ 0 };

		// Sets how long the lifetime of the particle should be
		std::chrono::microseconds maxLifetime{ 0 };

		// Determines whether to loop forever or stop generation after the lifespan runs out
		bool looping{ false };

		// Max number of particles that will be generated at any given time. If you start encroaching on this number
		// the generation will overwrite the old particles to generate new ones (probably). This may change to just not generate
		std::uint32_t maxParticles{ 10000 };

		// Texture that will be used for the particles. This will be given to each particle (most likely as a reference, which means it can't die until all of its particles are dead)
		Texture2D texture;

		// How long it should wait to start generating particles after creating the particle group
		std::chrono::microseconds startDelay{ 0 };

		// Possible bounds where a particle could spawn. If you want all on one point emission, you should use
		// a glm::vec2 of zeros. If you want a square area, glm::vec2 of 1s multiplied by the size of the square you want
		glm::vec2 emissionArea{ 0.0f };

		// Defined in degrees with 0 being aligned with the heading
		// Will default to 0, 360 which is just a full circle around itself
		glm::vec2 emissionArc{ 0.0f };

		// This controls how many particles are generated in the update loops
		std::uint32_t rateOverTime{ 0 };

		// List of particles in the particle group. This is helpful for actually updating the locations of every individual particle
		std::vector<Particle> particles{};

		// Tracks what index isn't being used yet in the particles vector (or if it has died, etc.)
		int unusedParticleIndex{ 0 };

		// Tracks whether the particle group has been preallocated.
		bool preallocated{ false };

	};
}
