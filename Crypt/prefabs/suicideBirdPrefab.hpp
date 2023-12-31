#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>

#include <components/controllerComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <components/rigidbodyComponent.hpp>
#include <singletons/time.hpp>
#include <components/mouseInputComponent.hpp>
#include <components/animationControllerComponent.hpp>
#include "../components/shootingComponent.hpp"
#include "../components/enemyDetectionComponent.hpp"
#include <components/cppScriptComponent.hpp>
#include "../scripts/shootingBatScript.hpp"
#include "../components/enemyComponent.hpp"
#include "healthBarPrefab.hpp"


namespace Crypt
{
	class SuicideBird
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, entities::EntityPtr player, std::function<void(entities::EntityPtr entity)> AddEntity)
		{
			entities::EntityPtr bird = std::make_shared<entities::Entity>();

			float detectionRange = 400.0f;
			float movementRange = 500.0f;
			float movementSpeed = 500.0f;
			glm::vec2 offset = { 0.0f, 0.0f };

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("bat"), Ebony::Colors::White, 0.10f);
			scale *= glm::vec2(sprite->texture->Width, sprite->texture->Height);

			components::Subcollider aabbcollider = components::Subcollider(scale / glm::vec2(2.0f, 2.0f), scale, true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::PLAYER_BULLET && !(other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::GROUND))
					{
						auto enemyInformation = bird->getComponent<components::Enemy>();

						enemyInformation->health -= other->getComponent<components::Bullet>()->strength;

						if (enemyInformation->health <= 0.0f)
						{
							bird->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						}

						other->getComponent<components::DestructionComponent>()->shouldDestroy = true;
					}
					else if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::GROUND)
					{
						bird->getComponent<components::DestructionComponent>()->shouldDestroy = true;
					}
				};

			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::ENEMY, Crypt::CollisionLayers::GROUND | Crypt::CollisionLayers::PLAYER_BULLET, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();

			//std::unique_ptr<components::CppScript> script = std::make_unique<scripts::ShootingBatScript>(AddEntity);

			bird->addComponent(std::make_unique<components::Enemy>(1.0f));
			bird->addComponent(std::make_unique<components::EnemyDetection>(detectionRange, movementRange, movementSpeed, offset, 400.0f, "bat_attack", player));

			bird->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					// Eventually, this is where the animations will be inserted and played probably (Maybe just spawn an entity or something)
					bird->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			//bird->addComponent(std::move(script));
			bird->addComponent(std::move(collider));
			bird->addComponent(std::move(transform));
			bird->addComponent(std::move(sprite));
			bird->addComponent(std::move(rigidbody));



			entities::EntityPtr healthBar = HealthBar::Create(startTransform + glm::vec2(0.0f, -20.0f), glm::vec2(scale.x, 7.0f), bird);
			AddEntity(healthBar);

			return bird;

		}
	};
}