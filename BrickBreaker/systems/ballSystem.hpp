#pragma once

#include "systems/system.hpp"
#include "../components/ballComponent.hpp"
#include <components/rigidbodyComponent.hpp>
#include <components/transform.hpp>

namespace systems
{
	class BallSystem : public System
	{
	public:
		BallSystem() : System({ctti::unnamed_type_id<components::Ball>(), ctti::unnamed_type_id<components::Transform>(), ctti::unnamed_type_id<components::RigidBody>()})
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	protected:
		void updateImpl(std::chrono::microseconds elapsedTime);

	};
}