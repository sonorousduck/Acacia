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

		void OnCollisionStart(entities::EntityPtr other, std::chrono::microseconds elapsedTime) override
		{
			auto layer = other->getComponent<components::Collider>()->layer;

			if (layer & BrickBreaker::CollisionLayers::BALL)
			{
				Die();
			}

		}
	};
}