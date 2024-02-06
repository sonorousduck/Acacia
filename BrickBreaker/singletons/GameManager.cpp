#include "GameManager.hpp"



namespace BrickBreaker
{

	float GameManager::currentPoints{ 0 };
	float GameManager::previousPoints{ 0 };
	bool GameManager::isAttachedToPaddle{ true };

	std::uint8_t GameManager::currentLives{ 3 };
	std::uint32_t GameManager::brickCount{ 0 };

	void GameManager::addPoints(float additionalPoints)
	{
		currentPoints += additionalPoints;
	}

	float GameManager::getPoints()
	{
		//auto pointDifference = currentPoints - previousPoints;
		//previousPoints = currentPoints;

		/*if (!isAttachedToPaddle) 
		{
			pointDifference++;
		}*/

		//return pointDifference;
		return currentPoints;
	}

	void GameManager::ResetPoints()
	{
		currentPoints = 0.0f;
		isAttachedToPaddle = true;
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