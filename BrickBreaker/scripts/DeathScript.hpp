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
			Ebony::SystemManager::nextScreenEnum = BrickBreaker::ScreenEnum::MAIN_MENU;
		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			auto transform = entity->getComponent<components::Transform>();
			if (transform->position.y > 320 || transform->position.y < 0 || transform->position.x > 480 || transform->position.x < 0)
			{
				Die();
			}

		}
	};
}