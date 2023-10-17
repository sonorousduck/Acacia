#include "physicsSystem.hpp"
#include <algorithm>
#include <iostream>

namespace systems
{
	void PhysicsSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}


	void PhysicsSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		//quadtree = Ebony::Quadtree(QUADTREE_SIZE, QUADTREE_SIZE);
		quadtree.Clear();

		for (auto& [id, entity] : m_Entities)
		{
			auto rigidBody = entity->getComponent<components::RigidBody>();

			// Update physics


			// Collision Detection, if a collider exists
			if (entity->hasComponent<components::Collider>())
			{
				auto collider = entity->getComponent<components::Collider>();

				// Check for AABB collisions


				// Check collisions between the place it was and where it is now



				// If AABB collision, and preciseCollisions is enabled, check those as well for complete collision




				// Call collision callbacks

			}


		}
	}

	bool PhysicsSystem::isInterested(const entities::EntityPtr& entity)
	{
		
		
		// This has to have either the RigidBody, Transform and a Collider or just a RigidBody and Transform. Colliders only do not work
		// Basically, ensure it has a rigidbody and a transform
		auto& components = entity->getComponents();
		bool doICare = components.contains(ctti::unnamed_type_id<components::RigidBody>()) && components.contains(ctti::unnamed_type_id<components::Transform>());

		return doICare;

	}
}

