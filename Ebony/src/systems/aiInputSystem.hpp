#pragma once

#include "system.hpp"

#include "../components/aiInputComponent.hpp"


namespace systems
{
	class AIInputSystem : public System
	{
	public:
		AIInputSystem() : System({ ctti::unnamed_type_id<components::AiInput>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}