#pragma once

#include "systems/system.hpp"
#include "../components/lifeComponent.hpp"
#include "../components/scoreComponent.hpp"



// These are going to be used to update the text for the points and update the quantity of hearts. Will just pass in the entity pointers

namespace systems
{
	class LifePointSystem : public System
	{
	public:
		LifePointSystem() : System({ctti::unnamed_type_id<components::Life>(), ctti::unnamed_type_id<components::Score>()})
		{}


		void Update(std::chrono::microseconds elapsedTime) override;

	protected:
		virtual bool isInterested(const entities::EntityPtr& entity);

		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}
