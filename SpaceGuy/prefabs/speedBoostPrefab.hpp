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
	class SpeedBoost
	{
	public:
		// Give a score value so spawned enemies aren't just worth farming forever
		static entities::EntityPtr Create(glm::vec2 startTransform)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("speed_boost"), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto scale = glm::vec2(sprite->texture->Width, sprite->texture->Height);


			components::Subcollider aabbcollider = components::Subcollider(scale / 2.0f, scale, true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					auto playerInformation = other->getComponent<components::PlayerInformation>();

					playerInformation->hasSpeedBoost = true;

					if (playerInformation->speedBoostTime > 0.0f)
					{
						playerInformation->speedBoostTime += 15.0f;
					}
					else
					{
						playerInformation->speedBoostTime = 15.0f;
					}
					entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;
				};


			auto collider = std::make_unique<components::Collider>(aabbcollider, SpaceGuy::CollisionLayers::KEY, SpaceGuy::CollisionLayers::PLAYER, true);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();



			entity->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					// Eventually, this is where the animations will be inserted and played probably (Maybe just spawn an entity or something)
					entity->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(rigidbody));


			return entity;
		}

	};
}