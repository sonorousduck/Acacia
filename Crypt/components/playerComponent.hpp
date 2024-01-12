#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

namespace components
{
	class Player : public PolymorphicComparable<Component, Player>
	{
	public:
		Player()
		{}


		bool isOnGround = false;
		bool isSlow = false;
		bool isFast = false;
		bool switchedSpeeds = false;
		float smoothingTime = 0.0f;
		float totalCameraSmoothingTime = 50.0f;
		float returnSmoothingTime = 20.0f;
		bool gravityDown = true;
		std::int16_t health = 6;
		std::int16_t lastHealth = 6;
		float score = 0;

		float gravityCooldown = 0.0f;
		float gravityUsageCooldownResetTime = 1.0f; // This will be the reset time that the gravity cooldown gets set to when the player uses it 
	};
}
