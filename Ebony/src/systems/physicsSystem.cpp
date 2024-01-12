#include "physicsSystem.hpp"



namespace systems
{
	void PhysicsSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	// Overriding the remove to check if 
	void PhysicsSystem::RemoveEntity(entities::Entity::IdType entityId)
	{
		if (!staticQuadtree.shouldRebuild)
		{
			components::Collider* collider;
			if (m_Entities.find(entityId) != m_Entities.end() && m_Entities[entityId]->tryGetComponent<components::Collider>(collider) && collider->isStatic)
			{
				staticQuadtree.shouldRebuild = true;
				staticQuadtree.Clear();
			}
		}

		m_Entities.erase(entityId);
	}

	void PhysicsSystem::updateObject(std::chrono::microseconds elapsedTime, entities::EntityPtr entity)
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

		transform->SetPosition(transform->position + rigidBody->getVelocity() * time_ms + rigidBody->getAcceleration() * time_ms * time_ms);


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



		components::Collider* collider;
		// Collision Detection, if a collider exists
		if (entity->tryGetComponent<components::Collider>(collider))
		{
			// Check for AABB collisions
			std::vector<entities::EntityPtr> possibleCollisions = quadtree.GetPossibleCollisions(entity);
			std::vector<entities::EntityPtr> staticPossibleCollisions = staticQuadtree.GetPossibleCollisions(entity);
			possibleCollisions.insert(possibleCollisions.end(), staticPossibleCollisions.begin(), staticPossibleCollisions.end());


			for (std::uint16_t i = 0; i < possibleCollisions.size(); i++)
			{
				if (entity->getId() != possibleCollisions[i]->getId() && (collider->layersToCollideWith & possibleCollisions[i]->getComponent<components::Collider>()->layer))
				{
					components::CppScript* script;
					bool hasScript = entity->tryGetComponent<components::CppScript>(script);

					if (HasAABBCollision(entity, possibleCollisions[i]))
					{
						if (!collider->preciseSubcolliderDetection)
						{
							// On Collision Start
							if (!collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
							{
								collider->aabbCollider.isCollidingLastFrame = true;

								if (collider->aabbCollider.onCollisionStart.has_value())
								{
									collider->aabbCollider.onCollisionStart.value()(possibleCollisions[i], elapsedTime);
								}
								if (hasScript)
								{
									script->OnCollisionStart(possibleCollisions[i], elapsedTime);
								}

								collider->currentlyCollidingWith.insert(possibleCollisions[i]->getId());

							}

							// On Collision
							else if (collider->aabbCollider.isCollidingLastFrame && collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
							{
								if (collider->aabbCollider.onCollision.has_value())
								{
									collider->aabbCollider.onCollision.value()(possibleCollisions[i], elapsedTime);
								}
								if (hasScript)
								{
									script->OnCollision(possibleCollisions[i], elapsedTime);
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
					else
					{
						if (collider->currentlyCollidingWith.contains(possibleCollisions[i]->getId()))
						{
							collider->aabbCollider.isCollidingLastFrame = false;

							if (collider->aabbCollider.onCollisionEnd.has_value())
							{
								collider->aabbCollider.onCollisionEnd.value()(possibleCollisions[i], elapsedTime);
							}
							if (hasScript)
							{
								script->OnCollisionEnd(possibleCollisions[i], elapsedTime);

							}

							collider->currentlyCollidingWith.erase(possibleCollisions[i]->getId());
						}
					}
				}
			}

			possibleCollisions.clear();
			// TODO: Check collisions between the place it was and where it is now
		}
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
			else if (staticQuadtree.shouldRebuild)
			{
				staticQuadtree.Insert(entity);
			}
			
		}

		staticQuadtree.shouldRebuild = false;

		std::latch graphDone{ 1 };

		auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
			[&graphDone]()
			{
				graphDone.count_down();
			});


		for (auto& [id, entity] : m_Entities)
		{
			auto task = Ebony::ThreadPool::instance().createTask(
				taskGraph,
				[this, elapsedTime, entity]()
				{
					//std::cout << "Updated!" << std::endl;
					updateObject(elapsedTime, entity);
				}
			);
		}

		Ebony::ThreadPool::instance().submitTaskGraph(taskGraph);
		graphDone.wait();
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

