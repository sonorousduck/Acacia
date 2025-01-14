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
#include <components/animationControllerComponent.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include "../components/playerComponent.hpp"
#include "../components/enemyComponent.hpp"

#include <components/cppScriptComponent.hpp>
#include "healthBarPrefab.hpp"
#include "../scripts/suicideBatScript.hpp"
#include <misc/renderLayers.hpp>


namespace Crypt
{
	class SuicideBird
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, entities::EntityPtr player)
		{
			entities::EntityPtr bird = std::make_shared<entities::Entity>();

			float detectionRange = 400.0f;
			float movementRange = 500.0f;
			float movementSpeed = 350.0f;
			glm::vec2 offset = { 0.0f, 0.0f };
			std::uint16_t explosionStrength = 3;

			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("bat"), Ebony::Colors::White, 0.10f);
			//scale *= glm::vec2(sprite->texture->Width, sprite->texture->Height);

			auto animationController = std::make_unique<components::AnimationController>(Ebony::RenderLayer::FOREGROUND);

			auto spriteSheet = Ebony::ResourceManager::GetTexture("bomber_bird");

			std::vector<std::chrono::microseconds> timings(6, std::chrono::milliseconds(120));

			std::vector<components::Link> links = { components::Link(1, [=]() {
								return (false);

			}) };


			std::vector<Ebony::Animation> animations = { Ebony::Animation(SpriteSheet(spriteSheet, 6, timings)) };

			auto node1 = components::Node(links, animations);

			animationController->animationTree.emplace_back(node1);
			bird->addComponent(std::move(animationController));

			

			scale *= glm::vec2(spriteSheet->Width / 6, spriteSheet->Height); // We know there are 6 in the x


			components::Subcollider aabbcollider = components::Subcollider(scale / glm::vec2(2.0f, 2.0f), scale, true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::PLAYER_BULLET && !(other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::GROUND))
					{
						auto enemyInformation = bird->getComponent<components::Enemy>();

						enemyInformation->health -= other->getComponent<components::Bullet>()->strength;

						if (enemyInformation->health <= 0.0f)
						{
							player->getComponent<components::Player>()->score += 100.0f;
							bird->getComponent<components::DestructionComponent>()->shouldDestroy = true;

						}

						other->getComponent<components::DestructionComponent>()->shouldDestroy = true;
					}
					else if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::GROUND)
					{
						bird->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						bird->getComponent<components::Enemy>()->health = 0.0f;
					}
					else if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::PLAYER)
					{
						bird->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						bird->getComponent<components::Enemy>()->health = 0.0f;

						other->getComponent<components::Player>()->health -= explosionStrength;
					}
				};

			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::ENEMY, Crypt::CollisionLayers::GROUND | Crypt::CollisionLayers::PLAYER_BULLET | Crypt::CollisionLayers::PLAYER, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale, glm::vec3(0.0f, 1.0f, 0.0f));
			auto rigidbody = std::make_unique<components::RigidBody>();

			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::SuicideBatScript>();

			bird->addComponent(std::make_unique<components::Enemy>(1.0f));
			bird->addComponent(std::make_unique<components::EnemyDetection>(detectionRange, movementRange, movementSpeed, offset, 400.0f, "bat_attack", player, 0.0f, 180.0f));

			bird->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					Ebony::SystemManager::AddEntity(Crypt::ExplosionDeathPrefab::Create(bird->getComponent<components::Transform>()->position, glm::vec2(1.0f, 1.0f)));
					bird->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			bird->addComponent(std::move(script));
			bird->addComponent(std::move(collider));
			bird->addComponent(std::move(transform));
			//bird->addComponent(std::move(sprite));
			bird->addComponent(std::move(rigidbody));



			entities::EntityPtr healthBar = HealthBar::Create(startTransform + glm::vec2(0.0f, -20.0f), glm::vec2(scale.x, 7.0f), bird);
			Ebony::SystemManager::AddEntity(healthBar);

			return bird;

		}
	};
}