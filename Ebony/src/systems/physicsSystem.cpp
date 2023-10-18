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
				std::vector<entities::EntityPtr> possibleCollisions = quadtree.GetPossibleCollisions(entity);
				// TODO: Look into adding a static collisions

				std::vector<std::uint16_t> currentCollisions{};


				for (std::uint16_t i = 0; i < possibleCollisions.size(); i++)
				{
					if (HasAABBCollision(collider, possibleCollisions[i]->getComponent<components::Collider>()))
					{
						if (!collider->preciseSubcolliderDetection)
						{
							// On Collision Start
							if (!collider->aabbCollider.isCollidingLastFrame && !collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
							{
								if (collider->aabbCollider.onCollisionStart.has_value())
								{
									collider->aabbCollider.onCollisionStart.value()(entity, possibleCollisions[i]);
									collider->aabbCollider.isCollidingLastFrame = true;

									collider->currentlyCollidingWith.insert(possibleCollisions[i]->getId());
								}
							}

							// On Collision
							else if (collider->aabbCollider.isCollidingLastFrame && collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
							{
								if (collider->aabbCollider.onCollision.has_value())
								{
									collider->aabbCollider.onCollision.value()(entity, possibleCollisions[i]);
								}
							}

							// On Collision End
							else
							{
								if (collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
								{
									if (collider->aabbCollider.onCollisionEnd.has_value())
									{
										collider->aabbCollider.onCollisionEnd.value()(entity, possibleCollisions[i]);
									}
								}
							}
						}
						// If AABB collision, and preciseCollisions is enabled, check those as well for complete collision
						else
						{
							// Has collision will handle the Collision callbacks, since there will be specific callbacks for each subcollider
							HasCollision(collider, possibleCollisions[i]->getComponent<components::Collider>());
						}
					}
				}


				// TODO: Check collisions between the place it was and where it is now

			}


		}
	}

	bool PhysicsSystem::HasCollision(const components::Collider* collider, const components::Collider* otherCollider)
	{
		return false;
	}

	bool PhysicsSystem::HasAABBCollision(const components::Collider* collider, const components::Collider* otherCollider)
	{
		return false;
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

