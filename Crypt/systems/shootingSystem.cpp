#include "shootingSystem.hpp"
#include <singletons/time.hpp>


namespace systems
{
	void ShootingSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void ShootingSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto bullet = entity->getComponent<components::Shooting>();

			if (bullet->currentCooldown < bullet->maxShootingSpeed)
			{
				bullet->currentCooldown += Ebony::Time::GetDeltaTimeFloat();
			}


		}
	}
}

