#include "physicsSystem.hpp"
#include <algorithm>
#include <iostream>
#include "../../../BrickBreaker/misc/collisionLayers.hpp"

namespace systems
{
	void PhysicsSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	// Overriding the remove to check if 
	void PhysicsSystem::RemoveEntity(entities::Entity::IdType entityId)
	{
		if (!staticQuadtree.shouldRebuild && m_Entities[entityId]->getComponent<components::Collider>()->isStatic)
		{
			staticQuadtree.shouldRebuild = true;
			staticQuadtree.Clear();
		}

		m_Entities.erase(entityId);
	}


	void PhysicsSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		quadtree.Clear();

		for (auto& [id, entity] : m_Entities)
		{

			if (!entity->getComponent<components::Collider>()->isStatic)
			{
				quadtree.Insert(entity);
			}
			else
			{
				if (staticQuadtree.shouldRebuild)
				{
					staticQuadtree.Insert(entity);
				}
			}
		}

		staticQuadtree.shouldRebuild = false;


		for (auto& [id, entity] : m_Entities)
		{
			auto rigidBody = entity->getComponent<components::RigidBody>();
			auto transform = entity->getComponent<components::Transform>();
			transform->previousPosition = transform->position;
			auto time_ms = elapsedTime.count() / 1000000.0f;

			// Update physics
			// TODO: Update gravity, and other things
			if (rigidBody->usesGravity)
			{
				rigidBody->setAcceleration(rigidBody->getAcceleration() + (glm::vec2(0.0f, GRAVITY_CONSTANT) * time_ms * time_ms));
			}

			transform->position = transform->position + rigidBody->getVelocity() * time_ms + rigidBody->getAcceleration() * time_ms * time_ms;
			

			// Apply forces
			while (rigidBody->getForceLength() > 0)
			{
				// Apply Forces
			}



			// Apply velocity and acceleration to object position

			// Update velocity as well
			rigidBody->setVelocity(rigidBody->getVelocity() + rigidBody->getAcceleration() * time_ms);


			while (rigidBody->getNextScriptedMovementLength() > 0)
			{
				// Apply scripted movements
				auto scriptedMovement = rigidBody->getNextScriptedMovement();
				transform->position += scriptedMovement;
			}
			



			// Collision Detection, if a collider exists
			if (entity->hasComponent<components::Collider>())
			{
				auto collider = entity->getComponent<components::Collider>();

				if (collider->layer & BrickBreaker::CollisionLayers::BRICK)
				{
					//std::cout << "Brick" << std::endl;
				}

				// Check for AABB collisions
				std::vector<entities::EntityPtr> possibleCollisions = quadtree.GetPossibleCollisions(entity);
				std::vector<entities::EntityPtr> staticPossibleCollisions = staticQuadtree.GetPossibleCollisions(entity);
				possibleCollisions.insert(possibleCollisions.end(), std::make_move_iterator(staticPossibleCollisions.begin()), std::make_move_iterator(staticPossibleCollisions.end()));

				// TODO: Look into adding a static collisions


				for (std::uint16_t i = 0; i < possibleCollisions.size(); i++)
				{
					if (entity->getId() != possibleCollisions[i]->getId() && (collider->layer & possibleCollisions[i]->getComponent<components::Collider>()->layer))
					{
						if (HasAABBCollision(entity, possibleCollisions[i]))
						{
							if (!collider->preciseSubcolliderDetection)
							{
								// On Collision Start
								if (!collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
								{
									//collider->aabbCollider.isCollidingLastFrame = true;

									if (collider->aabbCollider.onCollisionStart.has_value())
									{
										collider->aabbCollider.onCollisionStart.value()(entity, possibleCollisions[i], elapsedTime);

										collider->currentlyCollidingWith.insert(possibleCollisions[i]->getId());
									}
								}

								// On Collision
								else if (collider->aabbCollider.isCollidingLastFrame && collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
								{
									if (collider->aabbCollider.onCollision.has_value())
									{
										collider->aabbCollider.onCollision.value()(entity, possibleCollisions[i], elapsedTime);
									}
								}

								// On Collision End
								else
								{
									//collider->aabbCollider.isCollidingLastFrame = false;

									if (collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
									{
										if (collider->aabbCollider.onCollisionEnd.has_value())
										{
											collider->aabbCollider.onCollisionEnd.value()(entity, possibleCollisions[i], elapsedTime);
										}
										collider->currentlyCollidingWith.erase(possibleCollisions[i]->getId());
									}
								}
							}
							// If AABB collision, and preciseCollisions is enabled, check those as well for complete collision
							else
							{
								// Has collision will handle the Collision callbacks, since there will be specific callbacks for each subcollider
								HasCollision(entity, possibleCollisions[i]);
							}
						}
					}
				}

				possibleCollisions.clear();
				// TODO: Check collisions between the place it was and where it is now

			}
		}
	}

	bool PhysicsSystem::HasCollision(const entities::EntityPtr entity, const entities::EntityPtr otherEntity)
	{
		return false;
	}

	bool PhysicsSystem::HasAABBCollision(const entities::EntityPtr& entity, const entities::EntityPtr& otherEntity)
	{
		components::Subcollider aabbCollider = entity->getComponent<components::Collider>()->aabbCollider;
		components::Subcollider otherAabbCollider = otherEntity->getComponent<components::Collider>()->aabbCollider;

		components::Transform* transform = entity->getComponent<components::Transform>();
		components::Transform* otherTransform = otherEntity->getComponent<components::Transform>();

		//auto check = aabbCollider.getSize();

		//auto test = transform->position.x - aabbCollider.getSize().x / 2.0f > otherTransform->position.x + otherAabbCollider.getSize().x / 2.0f;
		//auto test1 = transform->position.x + aabbCollider.getSize().x / 2.0f < otherTransform->position.x - otherAabbCollider.getSize().x / 2.0f;
		//auto test2 = transform->position.y - aabbCollider.getSize().y / 2.0f > otherTransform->position.y + otherAabbCollider.getSize().y / 2.0f;
		//auto test3 = transform->position.y + aabbCollider.getSize().y / 2.0f < otherTransform->position.y - otherAabbCollider.getSize().y / 2.0f;



		return !(
			transform->position.x + aabbCollider.getCenter().x - aabbCollider.getSize().x / 2.0f > otherTransform->position.x + otherAabbCollider.getCenter().x + otherAabbCollider.getSize().x / 2.0f || // aabb left is greater than otherAbb right
			transform->position.x + aabbCollider.getCenter().x +  aabbCollider.getSize().x / 2.0f < otherTransform->position.x + otherAabbCollider.getCenter().x - otherAabbCollider.getSize().x / 2.0f || // aabb right is less than otherAbb left
			transform->position.y + aabbCollider.getCenter().y - aabbCollider.getSize().y / 2.0f > otherTransform->position.y + otherAabbCollider.getCenter().y + otherAabbCollider.getSize().y / 2.0f || // aabb top is below otherAbb bottom
			transform->position.y + aabbCollider.getCenter().y + aabbCollider.getSize().y / 2.0f < otherTransform->position.y + otherAabbCollider.getCenter().y - otherAabbCollider.getSize().y / 2.0f    // aabb bottom is above otherAbb top
			);
	}

	bool PhysicsSystem::isInterested(const entities::EntityPtr& entity)
	{
		
		
		// This has to have either the RigidBody, Transform and a Collider or just a RigidBody and Transform. Colliders only do not work
		// Basically, ensure it has a rigidbody and a transform
		auto& components = entity->getComponents();
		bool doICare = components.contains(ctti::unnamed_type_id<components::RigidBody>()) && components.contains(ctti::unnamed_type_id<components::Transform>());

		// TODO: DO THIS FOR STATIC OBJECTS
		//if (components.contains(ctti::unnamed_type_id<components::Collider>()))
		//{

		//}

		return doICare;

	}
}

