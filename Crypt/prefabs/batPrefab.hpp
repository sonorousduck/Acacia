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
	class Bat
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, entities::EntityPtr player, std::function<void(entities::EntityPtr entity)> AddEntity)
		{
			entities::EntityPtr bat = std::make_shared<entities::Entity>();


			float detectionRange = 300.0f;
			float movementRange = 500.0f;
			float movementSpeed = 200.0f;
			glm::vec2 offset = { 200.0f, 50.0f };

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("bat"), Ebony::Colors::White, 0.10f);
			scale *= glm::vec2(sprite->texture->Width, sprite->texture->Height);


			components::Subcollider aabbcollider = components::Subcollider(scale / glm::vec2(2.0f, 2.0f), scale, true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					auto otherLayer = other->getComponent<components::Collider>()->layer;

					if (otherLayer & Crypt::CollisionLayers::PLAYER_BULLET)
					{
						auto enemyInformation = bat->getComponent<components::Enemy>();

						enemyInformation->health -= other->getComponent<components::Bullet>()->strength;

						if (enemyInformation->health <= 0.0f)
						{
							bat->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						}

						other->getComponent<components::DestructionComponent>()->shouldDestroy = true;
					}
					if (otherLayer & CollisionLayers::ENEMY)
					{
						bat->getComponent<components::Transform>()->position = bat->getComponent<components::Transform>()->previousPosition;
					}
				};

			aabbcollider.onCollision = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					auto otherLayer = other->getComponent<components::Collider>()->layer;

					if (otherLayer & CollisionLayers::ENEMY)
					{
						bat->getComponent<components::Transform>()->position = bat->getComponent<components::Transform>()->previousPosition;
					}
				};

			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::ENEMY, Crypt::CollisionLayers::PLAYER_BULLET | CollisionLayers::GROUND | CollisionLayers::ENEMY, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale, glm::vec3(0.0f, 1.0f, 0.0f));
			auto rigidbody = std::make_unique<components::RigidBody>();

			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::ShootingBatScript>(AddEntity);


			auto shootingComponent = std::make_unique<components::Shooting>();


			bat->addComponent(std::make_unique<components::Enemy>(3.0f));
			bat->addComponent(std::make_unique<components::EnemyDetection>(detectionRange, movementRange, movementSpeed, offset, 400.0f, "bat_attack", player, 5.0f));

			bat->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					// Eventually, this is where the animations will be inserted and played probably (Maybe just spawn an entity or something)
					bat->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			bat->addComponent(std::move(script));
			bat->addComponent(std::move(shootingComponent));
			bat->addComponent(std::move(collider));
			bat->addComponent(std::move(transform));
			bat->addComponent(std::move(sprite));
			bat->addComponent(std::move(rigidbody));


			entities::EntityPtr healthBar = HealthBar::Create(startTransform + glm::vec2(0.0f, -20.0f), glm::vec2(scale.x, 7.0f), bat);
			AddEntity(healthBar);

			return bat;
		}

	};
}