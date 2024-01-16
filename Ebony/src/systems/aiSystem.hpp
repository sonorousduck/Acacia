#pragma once

#include "system.hpp"
#include "../components/aiComponent.hpp"

// NOTE: THESE ARE PLACEHOLDERS TO USE IN INDIVIDUAL GAMES! SINCE STATE IS DEPENDENT ON THE GAME
// YOU CAN'T HAVE A GENERALIZED AI SYSTEM

namespace systems
{
	class BaseAISystem : public System
	{
	public:
		BaseAISystem() : System({ ctti::unnamed_type_id<components::AIComponent>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}