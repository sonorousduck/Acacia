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
#include <singletons/inputManager.hpp>
#include <singletons/systemManager.hpp>
#include <components/mouseInputComponent.hpp>
#include "../prefabs/UI/playerMissilePrefab.hpp"
#include "../prefabs/UI/playerLivesPrefab.hpp"
#include <components/aiComponent.hpp>
#include "../singletons/SpaceGuyPythonManager.hpp"


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


			auto transform = std::make_unique<components::Transform>(glm::vec2(560.0f, 1000.0f), 0.0f, scale);
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
						auto transform = entity->getComponent<components::Transform>();

						// Decide which direction they came from by subtracting the positions, then just take a little bit more so it moves them slightly more away from the wall
						auto direction = transform->position - transform->previousPosition;



						transform->position = transform->previousPosition;
						transform->previousPosition = transform->position;
						
					}
					
				};

		

			auto collider = std::make_unique<components::Collider>(aabbcollider, 
				SpaceGuy::CollisionLayers::PLAYER, 
				SpaceGuy::CollisionLayers::ENEMY | SpaceGuy::CollisionLayers::WALL | SpaceGuy::CollisionLayers::ENEMY_BULLET | SpaceGuy::CollisionLayers::KEY | SpaceGuy::CollisionLayers::SENSOR_REGION, 
				false);

			auto keyboardComponent = std::make_unique<components::KeyboardInput>();
			auto controllerComponent = std::make_unique<components::ControllerInput>(0);
			auto mouseComponent = std::make_unique<components::MouseInput>();
			auto aiInputComponent = std::make_unique<components::AiInput>();

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

			if (!Ebony::SystemManager::aiEnabled)
			{
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
			}
			
			
			mouseComponent->bindings.insert({ SDL_BUTTON_LEFT, "shootLaser" });
			mouseComponent->bindings.insert({ SDL_BUTTON_RIGHT, "shootMissile" });

			mouseComponent->onPressActions.insert({ "shootLaser",[=](Ebony::MousePress& mousePress)
				{
					auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();
					
					if (playerShootingInformation->canShoot)
					{
						auto transform = entity->getComponent<components::Transform>();
						Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation, 1.0f, "player_bullet", "laser_shoot"));
						if (playerShootingInformation->hasShotgun)
						{
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation + 15, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation + 7.5f, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation - 15, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation - 7.5f, 1.0f, "player_bullet", "laser_shoot", false));
						}

						entity->getComponent<components::TimedComponent>()->ResetTimer();
						playerShootingInformation->canShoot = false;

					}
				} 
			});

			mouseComponent->onHeldActions.insert({ "shootLaser",[=](Ebony::MousePress& mousePress)
				{
					auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();

					if (playerShootingInformation->canShoot)
					{
						auto transform = entity->getComponent<components::Transform>();
						Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation, 1.0f, "player_bullet", "laser_shoot"));

						if (playerShootingInformation->hasShotgun)
						{
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation + 15, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation + 7.5f, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation - 15, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation - 7.5f, 1.0f, "player_bullet", "laser_shoot", false));

						}
						playerShootingInformation->canShoot = false;
						entity->getComponent<components::TimedComponent>()->ResetTimer();
					}
				}
				});

			mouseComponent->onPressActions.insert({ "shootMissile",[=](Ebony::MousePress& mousePress)
				{
					auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();

					if (playerShootingInformation->missileCount > 0)
					{
						auto transform = entity->getComponent<components::Transform>();
						Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation, 3.0f, "missile", "missile_shoot"));

						entity->getComponent<components::TimedComponent>()->ResetTimer();
						playerShootingInformation->missileCount -= 1;
					}
				}
			});

			aiInputComponent->actions.insert({ "shootLaser", [=]() {
				auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();

					if (playerShootingInformation->canShoot)
					{
						auto transform = entity->getComponent<components::Transform>();
						Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation, 1.0f, "player_bullet", "laser_shoot"));
						if (playerShootingInformation->hasShotgun)
						{
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation + 15, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation + 7.5f, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation - 15, 1.0f, "player_bullet", "laser_shoot", false));
							Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation - 7.5f, 1.0f, "player_bullet", "laser_shoot", false));
						}

						entity->getComponent<components::TimedComponent>()->ResetTimer();
						playerShootingInformation->canShoot = false;

					}
			} });

			aiInputComponent->actions.insert({ "shootMissile", [=]() {
				auto playerShootingInformation = entity->getComponent<components::PlayerShootingInformation>();

					if (playerShootingInformation->missileCount > 0)
					{
						auto transform = entity->getComponent<components::Transform>();
						Ebony::SystemManager::AddEntity(SpaceGuy::PlayerBullet::Create(transform->position, transform->rotation, 3.0f, "missile", "missile_shoot"));

						entity->getComponent<components::TimedComponent>()->ResetTimer();
						playerShootingInformation->missileCount -= 1;
					}
			} });

			aiInputComponent->actions.insert({ "move", [=]() {
				auto& action = SpaceGuy::SpaceGuyPythonManager::action;

					if (action.action[0] > 0)
					{
						// Move up by amount * speed
						auto playerInformation = entity->getComponent<components::PlayerInformation>();
						entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(0.0f, -playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f)));
					}
					else
					{
						// Move down by amount * speed
						auto playerInformation = entity->getComponent<components::PlayerInformation>();
						entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2{ 0.0f, playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f) });
					}

					if (action.action[1] > 0)
					{
						// Move right by amount * speed
						auto playerInformation = entity->getComponent<components::PlayerInformation>();
						entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f), 0.0f));
					}
					else
					{
						// Move left by amount * speed
						auto playerInformation = entity->getComponent<components::PlayerInformation>();
						entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(-playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() * (playerInformation->hasSpeedBoost ? playerBoostSpeed : 1.0f), 0.0f));
					}
			} });

			aiInputComponent->actions.insert({ "aimShip", [=]() {
					auto playerTransform = entity->getComponent<components::Transform>();
					auto& action = SpaceGuy::SpaceGuyPythonManager::action;

					auto& angle = action.action[2];
					angle *= 360;
					entity->getComponent<components::Transform>()->rotation = angle;
			} });


			aiInputComponent->translationFunction = [=]() 
				{
					auto& action = SpaceGuy::SpaceGuyPythonManager::action;

					if (action.action[3] >= 0.5)
					{
						entity->getComponent<components::AiInput>()->actions["shootLaser"]();
					}
					if (action.action[4] >= 0.5)
					{
						entity->getComponent<components::AiInput>()->actions["shootMissile"]();
					}

					entity->getComponent<components::AiInput>()->actions["aimShip"]();
					entity->getComponent<components::AiInput>()->actions["move"]();

				};



			
			std::unique_ptr<components::CppScript> cameraFollowing = std::make_unique<scripts::FollowPlayerCamera>();

			auto aiComponent = std::make_unique<components::AIComponent>(Ebony::AIType::STATE | Ebony::AIType::REWARD, SpaceGuy::AiInformationTypes::PLAYER_INFORMATION);

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
			entity->addComponent(std::move(aiInputComponent));
			entity->addComponent(std::move(aiComponent));


			Ebony::SystemManager::AddEntity(SpaceGuy::PlayerScore::Create(480, entity));
			Ebony::SystemManager::AddEntity(SpaceGuy::PlayerHealth::Create(320, entity));
			Ebony::SystemManager::AddEntity(SpaceGuy::PlayerMissilePrefab::Create(glm::vec2(128.0f, 304.0f), entity));
			Ebony::SystemManager::AddEntity(SpaceGuy::PlayerLifePrefab::Create(glm::vec2(164.0f, 304.0f), entity));

			return entity;
		}
	};
}