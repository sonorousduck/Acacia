#pragma once

#include <glm/glm.hpp>
#include <chrono>

#include "component.hpp"
#include <colors.hpp>
#include <texture.hpp>
#include "shader.hpp"

#include <random>

//struct Shape
//{
//
//};


struct Particle
{
	Particle(Texture2D& texture, std::chrono::microseconds lifetime, std::chrono::microseconds alive) : texture(texture), lifetime(lifetime), alive(alive) {};

	Particle(Texture2D& texture, std::chrono::microseconds lifetime, glm::vec2 startSize, glm::vec2 endSize, float startAlpha, float endAlpha) : texture(texture), lifetime(lifetime), startSize(startSize),
		endSize(endSize), startAlpha(startAlpha), endAlpha(endAlpha), alive(std::chrono::microseconds::zero()), currentAlpha(startAlpha), currentSize(startSize), currentColor(Ebony::Colors::White)
	{
		lerpColor = startColor != endColor;
		lerpAlpha = startAlpha != endAlpha;
		lerpSize = startSize != endSize;
	};
	Particle(Texture2D& texture, std::chrono::microseconds lifetime, glm::vec2 startSize, glm::vec2 endSize, float startAlpha, float endAlpha, Ebony::Color startColor, Ebony::Color endColor) : texture(texture), lifetime(lifetime), startSize(startSize),
		endSize(endSize), startAlpha(startAlpha), endAlpha(endAlpha), alive(std::chrono::microseconds::zero()), currentAlpha(startAlpha), currentSize(startSize), currentColor(startColor), startColor(startColor), endColor(endColor)
	{
		lerpColor = startColor != endColor;
		lerpAlpha = startAlpha != endAlpha;
		lerpSize = startSize != endSize;
	};

	// No way this worked... components::ParticleGroup doesn't work because it hasn't been defined yet
	// But since auto gets its type assigned at compile time, it actually works
	// Seems really dangerous to use this way.. :)
	Particle(auto* particleGroup)
	{
		texture = particleGroup->texture;
		lifetime = std::chrono::microseconds::zero();
		endSize = particleGroup->endSize;
		startAlpha = particleGroup->startAlpha;
		endAlpha = particleGroup->endAlpha;
		alive = std::chrono::microseconds::zero();
		currentAlpha = startAlpha;
		startColor = particleGroup->startColor;
		currentColor = startColor;
		endColor = particleGroup->endColor;


		if (particleGroup->randomStartSpeed)
		{
			auto xValue = particleGroup->random_double() * (particleGroup->maxStartSpeed.x - particleGroup->startSpeed.x) + particleGroup->startSpeed.x;
			auto yValue = particleGroup->random_double() * (particleGroup->maxStartSpeed.y - particleGroup->startSpeed.y) + particleGroup->startSpeed.y;

			startSpeed = glm::vec2(xValue, yValue);
			endSpeed = startSpeed;
			currentSpeed = startSpeed;
		}
		else
		{
			startSpeed = particleGroup->startSpeed;
			currentSpeed = startSpeed;
			endSpeed = particleGroup->endSpeed;
		}
		if (particleGroup->randomStartSize)
		{
			auto xValue = particleGroup->random_double() * (particleGroup->maxStartSize.x - particleGroup->startSize.x) + particleGroup->startSize.x;
			auto yValue = particleGroup->random_double() * (particleGroup->maxStartSize.y - particleGroup->startSize.y) + particleGroup->startSize.y;


			startSize = glm::vec2(xValue, yValue);
			currentSize = startSize;
			endSize = startSize;

		}
		else
		{
			startSize = particleGroup->startSize;
			currentSize = particleGroup->startSize;
			endSize = particleGroup->endSize;
		}

		lerpColor = startColor != endColor;
		lerpAlpha = startAlpha != endAlpha;
		lerpSize = startSize != endSize;
	}



	Particle(Texture2D& texture, std::chrono::microseconds lifetime, std::chrono::microseconds alive, glm::vec2 position, glm::vec2 direction, glm::vec2 velocity) :
		texture(texture), lifetime(lifetime), alive(alive), position(position), direction(direction), velocity(velocity) {};

	std::optional<Shader> shader;

	// Allows for fade in/fade out, etc.
	float startAlpha{ 1.0f };
	float endAlpha{ 1.0f };
	float currentAlpha{ 1.0f };

	// Allows control for the sizing of your particle through its life
	glm::vec2 startSize{ 0.0f };
	glm::vec2 endSize{ 0.0f };
	glm::vec2 currentSize{ 0.0f };

	glm::vec2 startSpeed{ 0.0f };
	glm::vec2 endSpeed{ 0.0f };
	glm::vec2 currentSpeed{ 0.0f };

	glm::vec2 position{ 0.0f };
	glm::vec2 direction{ 0.0f };
	glm::vec2 velocity{ 0.0f };

	float rotationRate{ 0.0f };
	float rotation{ 0.0f };

	bool lerpColor = false;
	bool lerpSize = false;
	bool lerpAlpha = false;

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
		ParticleGroup(Texture2D& texture, glm::vec2 velocity, float emissionArc, std::uint32_t maxParticles = 5000) : 
			texture(texture), 
			maxParticles(maxParticles), 
			startSpeed(velocity),
			maxStartSpeed(velocity),
			emissionArc(emissionArc)
		{
			static const GLfloat g_vertex_buffer_data[] = {
				-1.0f,  1.0f,
				-1.0f, -1.0f,
				 1.0f, -1.0f,

				-1.0f,  1.0f,
				 1.0f, -1.0f,
				 1.0f,  1.0f,
			};

			static const GLfloat g_uv_buffer_data[] = {
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f,

				0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 1.0f
			};

			//static const GLfloat g_uv_buffer_data[] = {
			//	0.0f, 0.0f,
			//	1.0f, 0.0f,
			//	1.0f, 1.0f,
			//	0.0f, 1.0f,
			//};

			glGenVertexArrays(1, &this->instancedVAO);
			glBindVertexArray(this->instancedVAO);

			// Allocate for the vertices
			glGenBuffers(1, &particleVertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particleVertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

			// Allocate for the Uvs
			glGenBuffers(1, &particleUvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particleUvBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

			// Initialize buffer with empty buffer, since it will be updated later at each frame
			glGenBuffers(1, &particlePositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(maxParticles) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

			glGenBuffers(1, &particleColorBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(maxParticles) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);


			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, particleVertexBuffer);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, particleUvBuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glVertexAttribDivisor(0, 0); // Always use the same vertices
			glVertexAttribDivisor(1, 0); // Always use the same uvs
			glVertexAttribDivisor(2, 1); // Positions: One per quad
			glVertexAttribDivisor(3, 1); // Color: One per quad


			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}



		// Enforcing maxParticles so we don't have to remake the buffers as often
		ParticleGroup(Texture2D& texture, std::uint32_t maxParticles = 5000) : texture(texture), maxParticles(maxParticles) 
		{

			//static const GLfloat g_vertex_buffer_data[] = {
			//	 -0.5f, -0.5f,
			//	 0.5f, -0.5f, 
			//	 -0.5f, 0.5f, 
			//	 0.5f, 0.5f,
			//};

			static const GLfloat g_vertex_buffer_data[] = {
				-1.0f,  1.0f,
				-1.0f, -1.0f,
				 1.0f, -1.0f,

				-1.0f,  1.0f,
				 1.0f, -1.0f,
				 1.0f,  1.0f,
			};

			static const GLfloat g_uv_buffer_data[] = {
				0.0f, 1.0f,
				0.0f, 0.0f,
				1.0f, 0.0f,

				0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 1.0f
			};

			//static const GLfloat g_uv_buffer_data[] = {
			//	0.0f, 0.0f,
			//	1.0f, 0.0f,
			//	1.0f, 1.0f,
			//	0.0f, 1.0f,
			//};

			glGenVertexArrays(1, &this->instancedVAO);
			glBindVertexArray(this->instancedVAO);

			// Allocate for the vertices
			glGenBuffers(1, &particleVertexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particleVertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

			// Allocate for the Uvs
			glGenBuffers(1, &particleUvBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particleUvBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

			// Initialize buffer with empty buffer, since it will be updated later at each frame
			glGenBuffers(1, &particlePositionBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(maxParticles) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

			glGenBuffers(1, &particleColorBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
			glBufferData(GL_ARRAY_BUFFER, static_cast<unsigned long long>(maxParticles) * 4 * sizeof(float), NULL, GL_STREAM_DRAW);


			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, particleVertexBuffer);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, particleUvBuffer);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);


			glEnableVertexAttribArray(3);
			glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glVertexAttribDivisor(0, 0); // Always use the same vertices
			glVertexAttribDivisor(1, 0); // Always use the same uvs
			glVertexAttribDivisor(2, 1); // Positions: One per quad
			glVertexAttribDivisor(3, 1); // Color: One per quad


			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

		};

		// Allow the developer to set a shader, if desired. If none is found, then a default particle shader will be used
		std::optional<Shader> shader;

		// For now, while there isn't a proper GameObject, the particle group will have a position
		glm::vec2 position{ 0.0f };

		// This defines the base velocity that each particle will be generated around
		glm::vec2 velocity{ 0.0f };

		// Tracks how long the particle group has been generating particles
		std::chrono::microseconds duration{ 0 };

		// Allows the particle group to stop generating new particles after this amount of time (0 means always generate)
		std::chrono::microseconds maxDuration{ 0 };

		// Sets how long the lifetime of the particle should be. Defaults to 5 seconds
		std::chrono::microseconds maxLifetime{ 5000 };

		// Determines whether to loop forever or stop generation after the lifespan runs out
		bool looping{ false };

		std::uint32_t getMaxParticles()
		{
			return maxParticles;
		}

		// Also need to redo the buffers, so this is an expensive operation
		void setMaxParticles(std::uint32_t newMax)
		{
			maxParticles = newMax;
			
			// TODO: Test to see if this even works
			
			glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);

			// Initialize buffer with empty buffer, since it will be updated later at each frame.
			glBufferData(GL_ARRAY_BUFFER, maxParticles * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, particlePositionBuffer);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glVertexAttribDivisor(1, 1); // Positions: One per quad
		}

		

		// Allows for fade in/fade out, etc.
		float startAlpha{ 1.0f };
		float endAlpha{ 1.0f };

		// Allows control for the sizing of your particle through its life
		glm::vec2 startSize{ 1.0f };
		glm::vec2 endSize{ 1.0f };

		// Random size generation
		bool randomStartSize = false;
		glm::vec2 maxStartSize{1.0f};

		bool randomStartSpeed = false;
		glm::vec2 startSpeed{ 1.0f };
		glm::vec2 maxStartSpeed{ 1.0f };

		glm::vec2 endSpeed{ 1.0f };

		
		Ebony::Color startColor{};
		Ebony::Color endColor{};

		std::uint32_t particleCount{ 0 };

		// Texture that will be used for the particles. This will be given to each particle (most likely as a reference, which means it can't die until all of its particles are dead)
		Texture2D& texture;

		// How long it should wait to start generating particles after creating the particle group
		std::chrono::microseconds startDelay{ 0 };

		// Possible bounds where a particle could spawn. If you want all on one point emission, you should use
		// a glm::vec2 of zeros. If you want a square area, glm::vec2 of 1s multiplied by the size of the square you want
		glm::vec2 emissionArea{ 0.0f };

		// Defined in degrees with 0 being aligned with the heading
		// Will default to 0, 360 which is just a full circle around itself
		float emissionArc{ 0.0f };

		// This controls how many particles are generated in the update loops
		std::uint32_t rateOverTime{ 0 };

		// List of particles in the particle group. This is helpful for actually updating the locations of every individual particle
		std::vector<Particle> particles{};

		// Tracks what index isn't being used yet in the particles vector (or if it has died, etc.)
		int unusedParticleIndex{ 0 };

		// Tracks whether the particle group has been preallocated.
		bool preallocated{ false };

		std::uint32_t min = 0;
		std::uint32_t max = 100;

		std::random_device rd;


		unsigned int instancedVAO = 0;
		unsigned int particleVertexBuffer = 0;
		unsigned int particleUvBuffer = 0;
		unsigned int particlePositionBuffer = 0;
		unsigned int particleColorBuffer = 0;

		std::vector<float> particlePositionSizeData = {};
		std::vector<float> particleColorData = {};
		std::chrono::microseconds spawnRate = std::chrono::milliseconds(16);
		std::chrono::microseconds accumulatedTime = std::chrono::milliseconds(0);

		inline double random_double()
		{
			static std::uniform_real_distribution<double> distribution(0.0, 1.0);
			static std::mt19937 generator;
			return distribution(generator);
		}

		static std::unique_ptr<ParticleGroup> Line(Texture2D& texture, std::uint32_t maxParticles = 5000)
		{
			return std::make_unique<ParticleGroup>(texture, glm::vec2(10.0f, 0.0f), 0.0f, maxParticles);
		}

		static std::unique_ptr<ParticleGroup> Box(Texture2D& texture, std::uint32_t maxParticles = 5000)
		{
			return std::make_unique<ParticleGroup>(texture, glm::vec2(20.0f, 10.0f), 360.0f, maxParticles);
		}

		static std::unique_ptr<ParticleGroup> Circle(Texture2D& texture, std::uint32_t maxParticles = 5000)
		{
			return std::make_unique<ParticleGroup>(texture, glm::vec2(10.0f, 10.0f), 360.0f, maxParticles);
		}

		static std::unique_ptr<ParticleGroup> Cone(Texture2D& texture, float coneAngle, std::uint32_t maxParticles = 5000)
		{
			return std::make_unique<ParticleGroup>(texture, glm::vec2(10.0f, 5.0f), coneAngle, maxParticles);
		}

		private:

			//void Preallocate()
			//{
			//	particles.reserve(maxParticles);
			//	while (particles.size() < maxParticles)
			//	{
			//		particles.push_back(Particle(texture, std::chrono::microseconds::zero(), startSize, endSize, startAlpha, endAlpha, startColor, endColor));
			//	}
			//	preallocated = true;
			//}
			// Max number of particles that will be generated at any given time. If you start encroaching on this number
			// the generation will overwrite the old particles to generate new ones (probably). This may change to just not generate
			std::uint32_t maxParticles{ 1000 };
	};
}
