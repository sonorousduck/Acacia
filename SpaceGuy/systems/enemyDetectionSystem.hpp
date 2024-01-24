#pragma once

#include "systems/system.hpp"
#include "../components/enemyDetectionComponent.hpp"


namespace systems
{
	class EnemyDetectionSystem : public System
	{
	public:
		EnemyDetectionSystem() : System({ ctti::unnamed_type_id<components::EnemyDetection>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}