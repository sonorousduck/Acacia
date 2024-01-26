#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include <singletons/time.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <components/cppScriptComponent.hpp>
#include "../components/enemyInformation.hpp"
#include "healthBarPrefab.hpp"
#include <singletons/systemManager.hpp>
#include "../prefabs/explosionDeathPrefab.hpp"
#include <misc/renderLayers.hpp>
#include <components/aiComponent.hpp>
#include "../misc/aiInformationTypes.hpp"
#include "../components/playerInformation.hpp"
#include "../components/bulletComponent.hpp"
#include "../scripts/enemyMovementShootingScript.hpp"

namespace SpaceGuy
{
	class ProximityLockedDoor
	{
	public:
		// Give a score value so spawned enemies aren't just worth farming forever
		static entities::EntityPtr Create(entities::EntityPtr lockedDoor)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			auto parentTransform = lockedDoor->getComponent<components::Transform>();

			auto scale = parentTransform->scale * glm::vec2(10.0f, 10.0f);


			components::Subcollider aabbcollider = components::Subcollider(scale / 2.0f, scale, true, true);

			aabbcollider.onCollision = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					if (other->getComponent<components::PlayerInformation>()->hasKey)
					{
						lockedDoor->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						other->getComponent<components::PlayerInformation>()->hasKey = false;
					}
				};


			auto collider = std::make_unique<components::Collider>(aabbcollider, SpaceGuy::CollisionLayers::WALL, SpaceGuy::CollisionLayers::PLAYER, true);
			auto transform = std::make_unique<components::Transform>(parentTransform->position - glm::vec2(20.0f, 20.0f), parentTransform->rotation, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();



			entity->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					// Eventually, this is where the animations will be inserted and played probably (Maybe just spawn an entity or something)
					entity->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(rigidbody));


			return entity;
		}

	};
}