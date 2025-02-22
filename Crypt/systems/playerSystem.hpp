#pragma once

#include "systems/system.hpp"

#include "../components/playerComponent.hpp"
#include <graphics2d.hpp>
#include <iostream>


namespace systems
{
	class PlayerSystem : public System
	{
	public:
		PlayerSystem() : System({ ctti::unnamed_type_id<components::Player>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}