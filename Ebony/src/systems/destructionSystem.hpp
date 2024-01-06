#pragma once

#include "system.hpp"

#include "../components/destructionComponent.hpp"
#include "../singletons/systemManager.hpp"

namespace systems
{
	class DestructionSystem : public System
	{
	public:
		DestructionSystem() : System({ ctti::unnamed_type_id<components::DestructionComponent>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;



	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}