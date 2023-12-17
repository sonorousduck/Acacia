#include "GameManager.hpp"



namespace Crypt
{

	std::uint32_t GameManager::currentPoints{ 0 };
	std::uint8_t GameManager::currentLives{ 3 };

	void GameManager::addPoints(std::uint32_t additionalPoints)
	{
		currentPoints += additionalPoints;
	}

	std::uint32_t GameManager::getPoints()
	{
		return currentPoints;
	}

	void GameManager::heal(int healAmount)
	{
		currentLives += healAmount;
	}

	void GameManager::takeDamage(int damageAmount)
	{
		currentLives -= damageAmount;
	}

	std::uint8_t GameManager::getLives()
	{
		return currentLives;
	}

}