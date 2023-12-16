#pragma once

#include <iostream>
#include "../Ebony/src/components/cppScriptComponent.hpp"
#include "../BrickBreaker/screens/screenEnums.hpp"

namespace scripts
{
	struct DeathScript : components::CppScript
	{
	public:
		DeathScript(std::uint64_t& nextScreen) : nextScreen(nextScreen)
		{
		}

		void Die()
		{
			nextScreen = BrickBreaker::ScreenEnum::MAIN_MENU;
		}

		void OnCollisionStart(entities::EntityPtr self, entities::EntityPtr other, std::chrono::microseconds elapsedTime) override
		{
			Die();
		}


		std::uint64_t& nextScreen;


	};
}