#pragma once

#include "system.hpp"

#include "../components/destructionComponent.hpp"


namespace systems
{
	class DestructionSystem : public System
	{
	public:
		DestructionSystem() : System({ ctti::unnamed_type_id<components::DestructionComponent>() })
		{}

		DestructionSystem(std::function<void(entities::Entity::IdType entityId)> OnRemove) : System({ ctti::unnamed_type_id<components::DestructionComponent>() }), OnRemove(OnRemove)
		{}

		void Update(std::chrono::microseconds elapsedTime) override;


	

		std::function<void(entities::Entity::IdType entityId)> OnRemove;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}