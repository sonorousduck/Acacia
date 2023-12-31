#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <graphics2d.hpp>
#include <SDL.h>
#include <Log.hpp>
#include <components/controllerComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <components/rigidbodyComponent.hpp>
#include <singletons/time.hpp>
#include <components/mouseInputComponent.hpp>
#include <components/animationControllerComponent.hpp>
#include "../components/playerComponent.hpp"
#include "../screens/screenEnums.hpp"
#include "../components/bulletComponent.hpp"

namespace Crypt
{
	class Player
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, std::function<void(std::uint64_t)> setNextScreen)
		{
			entities::EntityPtr player = std::make_shared<entities::Entity>();

			glm::vec2 originalVelocity = glm::vec2(100.0f, 500.0f);
			glm::vec2 originalVelocityWithoutY = glm::vec2(originalVelocity.x, 0.0f);

			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();

			controllerInputComponent->joystickBindings.insert({ SDL_CONTROLLER_AXIS_LEFTX, "playerMovement" });

			controllerInputComponent->joystickActions.insert({ "playerMovement", [=](float value) {
				if (abs(value) > 0.10)
				{
					auto rigidBody = player->getComponent<components::RigidBody>();
					auto transform = player->getComponent<components::Transform>();
					auto collider = player->getComponent<components::Collider>();

					if (value < 0)
					{
						rigidBody->setVelocity(originalVelocityWithoutY + glm::vec2(60.0f * value, rigidBody->getVelocity().y));
					}
					else if (value > 0)
					{
						rigidBody->setVelocity(originalVelocityWithoutY + glm::vec2(150.0f * value, rigidBody->getVelocity().y));
					}
				}
			} });

			keyboardInputComponent->onReleaseActions.insert({ "playerLeft", [=]() 
			{ 
				auto playerComponent = player->getComponent<components::Player>();
				auto rigidBody = player->getComponent<components::RigidBody>();

				if (playerComponent->isSlow)
				{
					playerComponent->isSlow = false;
					rigidBody->setVelocity(rigidBody->getVelocity() + glm::vec2(60.0f, 0.0f));
				}
			} 
			});

			keyboardInputComponent->onReleaseActions.insert({ "playerRight", [=]()
			{
				auto playerComponent = player->getComponent<components::Player>();
				auto rigidBody = player->getComponent<components::RigidBody>();

				if (playerComponent->isFast)
				{
					playerComponent->isFast = false;
					rigidBody->setVelocity(rigidBody->getVelocity() - glm::vec2(150.0f, 0.0f));
				}
			}
			});

			

			controllerInputComponent->onPressActions.insert({"pause", [=]()
				{
					setNextScreen(ScreenEnum::PAUSE);
				} });

			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_START, "pause" });


			keyboardInputComponent->onHeldActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->bindings.insert({ SDLK_a, "playerLeft" });
			keyboardInputComponent->bindings.insert({ SDLK_d, "playerRight" });
			keyboardInputComponent->bindings.insert({ SDLK_ESCAPE, "pause" });

			keyboardInputComponent->onPressActions.insert({ "pause", [=]()
				{
					setNextScreen(ScreenEnum::PAUSE);
				} });


			keyboardInputComponent->onHeldActions.insert({ "playerLeft", [=]()
			{
				auto rigidBody = player->getComponent<components::RigidBody>();
				auto transform = player->getComponent<components::Transform>();
				auto collider = player->getComponent<components::Collider>();
				auto playerComponent = player->getComponent<components::Player>();

				if (!playerComponent->isSlow && !playerComponent->isFast)
				{
					playerComponent->isSlow = true;

					rigidBody->setVelocity(rigidBody->getVelocity() - glm::vec2(60.0f, 0.0f));
				}
			} });

			keyboardInputComponent->onHeldActions.insert({ "playerRight", [=]()
			{
				auto rigidBody = player->getComponent<components::RigidBody>();
				auto transform = player->getComponent<components::Transform>();
				auto collider = player->getComponent<components::Collider>();
				auto playerComponent = player->getComponent<components::Player>();

				if (!playerComponent->isFast && !playerComponent->isSlow)
				{
					playerComponent->isFast = true;

					rigidBody->setVelocity(rigidBody->getVelocity() + glm::vec2(150.0f, 0.0f));
				}
			}
				});

			keyboardInputComponent->bindings.insert({ SDLK_w, "flipGravity" });
			keyboardInputComponent->bindings.insert({ SDLK_SPACE, "flipGravity" });


			keyboardInputComponent->onPressActions.insert({ "flipGravity", [=]()
				{
				auto rigidBody = player->getComponent<components::RigidBody>();
				auto transform = player->getComponent<components::Transform>();
				auto collider = player->getComponent<components::Collider>();
				auto playerComponent = player->getComponent<components::Player>();

				if (playerComponent->gravityCooldown <= 0.0f)
				{
					playerComponent->gravityCooldown = playerComponent->gravityUsageCooldownResetTime;
					playerComponent->gravityDown = !playerComponent->gravityDown;

					transform->rotation = fmod(transform->rotation + 180.0f, 360.0f);
					//std::cout << (playerComponent->gravityDown ? "-1" : "1") << std::endl;
					rigidBody->setVelocity(glm::vec2(rigidBody->getVelocity().x, originalVelocity.y * (playerComponent->gravityDown ? 1 : -1)));
					rigidBody->setAcceleration(glm::vec2(rigidBody->getAcceleration().x, 200.0f * (playerComponent->gravityDown ? 1 : -1)));

					playerComponent->isOnGround = false;
				}
			}
				});

			player->addComponent(std::make_unique<components::Player>());

			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("paddle_0"), Ebony::Colors::White);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(30.0f, 30.0f), glm::vec2(60.0f, 60.0f), true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					Crypt::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					if (layer & Crypt::CollisionLayers::GROUND)
					{
						player->getComponent<components::Player>()->isOnGround = true;
					}
					else if (layer & Crypt::CollisionLayers::ENEMY_BULLET)
					{
						player->getComponent<components::Player>()->health -= other->getComponent<components::Bullet>()->strength;
					}

				};


			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::PLAYER, Crypt::CollisionLayers::GROUND | Crypt::CollisionLayers::ENEMY_BULLET, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(60.0f, 60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			
			// Will use a separate system for controlling gravity and such
			auto rigidbody = std::make_unique<components::RigidBody>(originalVelocity, glm::vec2(0.0f, 200.0f));
			auto animationController = std::make_unique<components::AnimationController>();


			std::vector<std::chrono::microseconds> timings(4, std::chrono::milliseconds(120));

			std::vector<components::Link> links = { components::Link(1, [=]() {
								return (false);
				
			}) };
			std::vector<Ebony::Animation> animations = { Ebony::Animation(SpriteSheet(Ebony::ResourceManager::GetTexture("character_run"), 4, timings)) };
			
			auto node1 = components::Node(links, animations);

			animationController->animationTree.emplace_back(node1);

			// Debugging only
			// ===================================================================
			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::White);
			//player->addComponent(std::move(sprite));


			// ===================================================================



			player->addComponent(std::move(collider));
			player->addComponent(std::move(animationController));
			player->addComponent(std::move(transform));
			player->addComponent(std::move(keyboardInputComponent));
			player->addComponent(std::move(controllerInputComponent));
			player->addComponent(std::move(rigidbody));

			return player;
		}

	};
}