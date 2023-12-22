#pragma once

#include "system.hpp"

#include "../components/timedComponent.hpp"


namespace systems
{
	class TimingSystem : public System
	{
	public:
		TimingSystem() : System({ ctti::unnamed_type_id<components::TimedComponent>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}