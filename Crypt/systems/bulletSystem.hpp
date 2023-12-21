#pragma once

#include "systems/system.hpp"
#include "../components/bulletComponent.hpp"


namespace systems
{
	class BulletSystem : public System
	{
	public:
		BulletSystem() : System({ ctti::unnamed_type_id<components::Bullet>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}