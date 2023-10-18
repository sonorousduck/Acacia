#pragma once
#include "system.hpp"
#include "../components/collider.hpp"
#include "../components/rigidbodyComponent.hpp"
#include "../misc/Quadtree.hpp"
#include "../components/transform.hpp"

namespace systems
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem() : System({ ctti::unnamed_type_id<components::Collider>(), ctti::unnamed_type_id<components::RigidBody>(), ctti::unnamed_type_id<components::Transform>()})
		{
		}

		void Update(std::chrono::microseconds elapsedTime) override;

	protected:
		virtual bool isInterested(const entities::EntityPtr& entity);

		void updateImpl(std::chrono::microseconds elapsedTime);
		
	private:
		std::uint16_t PHYSICS_DIMENSION_WIDTH = 1000;
		std::uint16_t PHYSICS_DIMENSION_HEIGHT = 1000;
		float GRAVITY_CONSTANT = -9.81f;
		std::uint16_t QUADTREE_SIZE = PHYSICS_DIMENSION_HEIGHT * 10;
		std::uint16_t QUADTREE_MIDPOINT = QUADTREE_SIZE / 2;

		// To avoid warnings, this has to be initialized... Which is unfortunate since it just gets initialized again later
		Ebony::Quadtree quadtree = Ebony::Quadtree(QUADTREE_SIZE, QUADTREE_SIZE);

		bool HasCollision(const components::Collider* collider, const components::Collider* otherCollider);
		bool HasAABBCollision(const components::Collider* collider, const components::Collider* otherCollider);
	};
}