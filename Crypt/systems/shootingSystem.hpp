#pragma once

#include "systems/system.hpp"
#include "../components/shootingComponent.hpp"


namespace systems
{
	class ShootingSystem : public System
	{
	public:
		ShootingSystem() : System({ ctti::unnamed_type_id<components::Shooting>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}