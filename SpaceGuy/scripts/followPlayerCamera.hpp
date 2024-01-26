#pragma once

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>

#include "../components/playerInformation.hpp"
#include "../components/playerShootingInformation.hpp"
#include <singletons/time.hpp>
#include <graphics2d.hpp>

namespace scripts
{
	struct FollowPlayerCamera : components::CppScript
	{
	public:
		FollowPlayerCamera()
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{	
			auto transform = entity->getComponent<components::Transform>();


			Ebony::Graphics2d::mainCamera->SetCameraPosition(transform->position - glm::vec2(240.0f, 155.0f));

			auto playerInformation = entity->getComponent<components::PlayerInformation>();
			if (playerInformation->hasSpeedBoost && playerInformation->speedBoostTime > 0.0f)
			{
				playerInformation->speedBoostTime -= Ebony::Time::GetDeltaTimeFloat();

				if (playerInformation->speedBoostTime <= 0.0f)
				{
					playerInformation->hasSpeedBoost = false;
				}
			}


			if (playerInformation->health <= 0.0f)
			{
				playerInformation->lives -= 1;

				if (playerInformation->lives <= 0)
				{
					if (Ebony::SystemManager::aiEnabled)
					{
						Ebony::SystemManager::shouldResetForAi = true;
					}
					else
					{
						Ebony::SystemManager::nextScreenEnum = SpaceGuy::ScreenEnum::GAME_OVER;
					}
				}
				else
				{
					playerInformation->health = playerInformation->maxHealth;
					playerInformation->hasSpeedBoost = false;
					playerInformation->score -= 100.0f;

					transform->position = glm::vec2(560.0f, 1000.0f);

					auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();
					playerShootingInformation->hasRapidFire = false;
					playerShootingInformation->hasShotgun = false;
					playerShootingInformation->missileCount = playerShootingInformation->missilesToGain;
					playerShootingInformation->canShoot = true;
					entity->getComponent<components::TimedComponent>()->maxTime = 0.25;

					Ebony::Graphics2d::mainCamera->SetCameraPosition(transform->position - glm::vec2(240.0f, 155.0f));
				}
			}



		}
	};
}