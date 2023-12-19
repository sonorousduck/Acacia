#pragma once

#include "system.hpp"

#include "../components/aiComponent.hpp"


namespace systems
{
	class AISystem : public System
	{
	public:
		AISystem() : System({ ctti::unnamed_type_id<components::AIComponent>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}