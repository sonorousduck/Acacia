#pragma once

#include <cstdint>


namespace BrickBreaker
{
	class GameManager
	{
	public:
		static void addPoints(float additionalPoints);
		static float getPoints();
		static void ResetPoints();
		static void heal(int healAmount);
		static void takeDamage(int damageAmount);
		static std::uint8_t getLives();

		static std::uint32_t brickCount;

		static float currentPoints;
		static bool isAttachedToPaddle;

	private:
		GameManager() {}

		static float previousPoints;

		static std::uint8_t currentLives;



	};
}