#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>

#include <components/keyboardComponent.hpp>
#include <components/controllerComponent.hpp>
#include <components/cppScriptComponent.hpp>

#include "../misc/collisionLayers.hpp"
#include <graphics2d.hpp>
#include "../scripts/followPlayerCamera.hpp"

#include <SDL.h>
#include <graphics2d.hpp>
#include <singletons/time.hpp>
#include "../components/playerInformation.hpp"
#include "../components/playerShootingInformation.hpp"
#include "../prefabs/playerBullet.hpp"
#include "../prefabs/UI/playerScorePrefab.hpp"
#include "../prefabs/UI/playerHealthPrefab.hpp"


namespace SpaceGuy
{
	class Player
	{
	public:
		static entities::EntityPtr Create()
		{
			auto playerBaseSpeed = 100.0f;
			auto playerBoostSpeed = 1.5f;



			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto texture = Ebony::ResourceManager::GetTexture("player");
			auto scale = texture->getDimensions();


			auto transform = std::make_unique<components::Transform>(glm::vec2(240.0f, 155.0f), 0.0f, scale);
			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto rigidbody = std::make_unique<components::RigidBody>();
			auto playerInformation = std::make_unique<components::PlayerInformation>();
			auto playerShootingInformation = std::make_unique<components::PlayerShootingInformation>();
			auto shootingDelay = std::make_unique<components::TimedComponent>(0.25f, [=]()
				{
					auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>()->canShoot = true;
				});

			components::Subcollider aabbcollider = components::Subcollider(scale / 2.0f, scale, true, true);


			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					SpaceGuy::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					if (layer & SpaceGuy::CollisionLayers::WALL)
					{
						//entity->getComponent<components::Transform>()->position = entity->getComponent<components::Collider>()->aabbCollider.lastCollisionLocation.y;
					}
					else if (layer & SpaceGuy::CollisionLayers::ENEMY_BULLET)
					{
						entity->getComponent<components::PlayerInformation>()->health -= other->getComponent<components::Bullet>()->strength;
					}
				};



			auto collider = std::make_unique<components::Collider>(aabbcollider, 
				SpaceGuy::CollisionLayers::PLAYER, 
				SpaceGuy::CollisionLayers::ENEMY | SpaceGuy::CollisionLayers::WALL | SpaceGuy::CollisionLayers::ENEMY_BULLET | SpaceGuy::CollisionLayers::KEY | SpaceGuy::CollisionLayers::SENSOR_REGION, 
				false);

			auto keyboardComponent = std::make_unique<components::KeyboardInput>();
			auto controllerComponent = std::make_unique<components::ControllerInput>(0);
			auto mouseComponent = std::make_unique<components::MouseInput>();

			keyboardComponent->bindings.insert({ SDLK_w, "moveUp" });
			keyboardComponent->bindings.insert({ SDLK_a, "moveLeft" });
			keyboardComponent->bindings.insert({ SDLK_d, "moveRight" });
			keyboardComponent->bindings.insert({ SDLK_s, "moveDown" });
			keyboardComponent->bindings.insert({ SDLK_ESCAPE, "pause" });

			keyboardComponent->onHeldActions.insert({ "moveUp", [=]()
			{
					auto playerInformation = entity->getComponent<components::PlayerInformation>();
					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(0.0f, -playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f)));
			}
			});
			keyboardComponent->onHeldActions.insert({ "moveDown", [=]()
			{
					auto playerInformation = entity->getComponent<components::PlayerInformation>();

					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2{ 0.0f, playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f) });
			}
			});
			keyboardComponent->onHeldActions.insert({ "moveLeft", [=]()
				{
					auto playerInformation = entity->getComponent<components::PlayerInformation>();

					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(-playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f), 0.0f));

				}
			});
			keyboardComponent->onHeldActions.insert({ "moveRight", [=]()
				{
					auto playerInformation = entity->getComponent<components::PlayerInformation>();

					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f), 0.0f));
				}
			});


			mouseComponent->onMove = [=]()
				{
					auto mouseInput = entity->getComponent<components::MouseInput>();
					auto playerTransform = entity->getComponent<components::Transform>();
					glm::vec3 cameraPosition = Ebony::Graphics2d::mainCamera->Position;

					glm::vec2 currentDirection = glm::normalize((glm::vec2(mouseInput->positionX, mouseInput->positionY) + glm::vec2(cameraPosition.x, cameraPosition.y) - playerTransform->position));
					//std::cout << currentDirection.x << ", " << currentDirection.y << std::endl;

					auto angle = glm::atan(currentDirection.x, -currentDirection.y);
					entity->getComponent<components::Transform>()->rotation = glm::degrees(angle);
				};
			
			mouseComponent->bindings.insert({ SDL_BUTTON_LEFT, "shootLaser" });
			mouseComponent->bindings.insert({ SDL_BUTTON_RIGHT, "shootMissile" });

			mouseComponent->onPressActions.insert({ "shootLaser",[=](Ebony::MousePress& mousePress)
				{
					auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();
					
					if (playerShootingInformation->canShoot)
					{
						auto transform = entity->getComponent<components::Transform>();
						Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation, 1.0f, "player_bullet", "laser_shoot"));
						entity->getComponent<components::TimedComponent>()->ResetTimer();
					}


				} 
			});

			mouseComponent->onPressActions.insert({ "shootMissile",[=](Ebony::MousePress& mousePress)
				{
					auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();

					if (playerShootingInformation->canShoot)
					{
						auto transform = entity->getComponent<components::Transform>();
						Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation, 3.0f, "missile", "missile_shoot"));

						entity->getComponent<components::TimedComponent>()->ResetTimer();
					}


				}
			});


			
			std::unique_ptr<components::CppScript> cameraFollowing = std::make_unique<scripts::FollowPlayerCamera>();


			entity->addComponent(std::move(keyboardComponent));
			entity->addComponent(std::move(controllerComponent));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(rigidbody));
			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(mouseComponent));
			entity->addComponent(std::move(cameraFollowing));
			entity->addComponent(std::move(playerInformation));
			entity->addComponent(std::move(playerShootingInformation));
			entity->addComponent(std::move(shootingDelay));


			Ebony::SystemManager::AddEntity(SpaceGuy::PlayerScore::Create(480, entity));
			Ebony::SystemManager::AddEntity(SpaceGuy::PlayerHealth::Create(320, entity));



			return entity;
		}
	};
}