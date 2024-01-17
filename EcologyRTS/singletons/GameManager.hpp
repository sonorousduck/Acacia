#pragma once

#include <cstdint>


namespace EcologyRTS
{
	class GameManager
	{
	public:
		static void addPoints(std::uint32_t additionalPoints);
		static std::uint32_t getPoints();
		static void heal(int healAmount);
		static void takeDamage(int damageAmount);
		static std::uint8_t getLives();

		

	private:
		GameManager() {}

		static std::uint32_t currentPoints;
		static std::uint8_t currentLives;



	};
}