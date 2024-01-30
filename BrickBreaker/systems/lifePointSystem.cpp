#include "lifePointSystem.hpp"
#include <algorithm>
#include <components/text.hpp>
#include "../singletons/GameManager.hpp"


namespace systems
{
	void LifePointSystem::Update(std::chrono::microseconds elapsedTime)
	{
		updateImpl(elapsedTime);
	}

	bool LifePointSystem::isInterested(const entities::EntityPtr& entity)
	{
		bool doICare = std::any_of(
			m_Interests.begin(), m_Interests.end(),
			[&entity](auto interest)
			{
				return entity->getComponents().contains(interest);
			});


		return doICare;
	}

	void LifePointSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			// Update score text
			if (entity->hasComponent<components::Score>())
			{
				entity->getComponent<components::Text>()->text = "Score: " + std::to_string(static_cast<int>(BrickBreaker::GameManager::getPoints()));
			}
			else // Update lives
			{
				
			}
		}


	}



}

