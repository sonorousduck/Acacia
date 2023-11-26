#include "GameManager.hpp"



namespace BrickBreaker
{

	float GameManager::currentPoints{ 0.0f };
	std::uint8_t GameManager::currentLives{ 3 };

	void GameManager::addPoints(float additionalPoints)
	{
		currentPoints += additionalPoints;
	}

	float GameManager::getPoints()
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