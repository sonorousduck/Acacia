#pragma once

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>

#include "../components/playerInformation.hpp"
#include <singletons/time.hpp>
#include <graphics2d.hpp>
#include <iostream>

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

			Ebony::Graphics2d::mainCamera->SetCameraPosition(glm::vec2(Ebony::Graphics2d::mainCamera->Position) + transform->position - transform->previousPosition);


			auto playerInformation = entity->getComponent<components::PlayerInformation>();
			if (playerInformation->hasSpeedBoost && playerInformation->speedBoostTime > 0.0f)
			{
				playerInformation->speedBoostTime -= Ebony::Time::GetDeltaTimeFloat();

				if (playerInformation->speedBoostTime <= 0.0f)
				{
					playerInformation->hasSpeedBoost = false;
				}
			}


		}
	};
}