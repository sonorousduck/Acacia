#pragma once

#include <iostream>
#include "../Ebony/src/components/cppScriptComponent.hpp"
#include "../BrickBreaker/screens/screenEnums.hpp"
#include <singletons/systemManager.hpp>

namespace scripts
{
	struct DeathScript : components::CppScript
	{
	public:
		DeathScript()
		{
		}

		void Die()
		{

			if (Ebony::SystemManager::aiEnabled)
			{
				Ebony::SystemManager::shouldResetForAi = true;
			}
			else
			{
				Ebony::SystemManager::nextScreenEnum = BrickBreaker::ScreenEnum::MAIN_MENU;
			}

		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			auto transform = entity->getComponent<components::Transform>();
			if (transform->position.y >= 340 || transform->position.y <= -20 || transform->position.x >= 520 || transform->position.x <= -20)
			{
				Die();
			}

		}
	};
}