#pragma once

#include "system.hpp"

#include "../components/collider.hpp"


namespace systems
{
	class CollisionSystem : public System
	{
	public:
		CollisionSystem() : System({ ctti::unnamed_type_id<components::Collider>() })
		{
		}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);

	};
}




