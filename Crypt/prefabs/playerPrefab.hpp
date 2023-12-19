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


namespace Crypt
{
	class Player
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, int windowWidth)
		{
			entities::EntityPtr player = std::make_shared<entities::Entity>();


			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();

			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_B, "print" });
			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_Y, "printRelease" });


			controllerInputComponent->onHeldActions.insert({ "print", [=](entities::EntityPtr) {std::cout << "Circle was called (OnHeld)" << std::endl; } });
			controllerInputComponent->onReleaseActions.insert({ "printRelease", [=](entities::EntityPtr) {std::cout << "Triangle was called" << std::endl; } });
			controllerInputComponent->joystickBindings.insert({ SDL_CONTROLLER_AXIS_LEFTX, "paddleMovement" });

			controllerInputComponent->joystickActions.insert({ "paddleMovement", [=](entities::EntityPtr entity, float value) {
				/*if (abs(value) > 0.10)
				{
					auto rigidBody = entity->getComponent<components::RigidBody>();
					auto transform = entity->getComponent<components::Transform>();
					auto collider = entity->getComponent<components::Collider>();

					if (value < 0 && transform->position.x > 0)
					{
						std::cout << Ebony::Time::GetDeltaTimeFloat() << std::endl;
						rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloat() * value, 0.0f });
					}
					else if (value > 0 && transform->position.x + transform->scale.x < windowWidth)
					{
						rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloat() * value, 0.0f });
					}
				}*/
			} });

			keyboardInputComponent->onReleaseActions.insert({ "print", [=](entities::EntityPtr) { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->onHeldActions.insert({ "print", [=](entities::EntityPtr) { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->bindings.insert({ SDLK_a, "paddleLeft" });
			keyboardInputComponent->bindings.insert({ SDLK_d, "paddleRight" });

			keyboardInputComponent->onHeldActions.insert({ "paddleLeft", [=](entities::EntityPtr entity)
			{
				auto rigidBody = entity->getComponent<components::RigidBody>();
				auto transform = entity->getComponent<components::Transform>();
				auto collider = entity->getComponent<components::Collider>();


				if (transform->position.x > 0)
				{
					rigidBody->addScriptedMovement(glm::vec2{ -700.0f * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				}
			} });

			keyboardInputComponent->onHeldActions.insert({ "paddleRight", [=](entities::EntityPtr entity)
			{
				auto rigidBody = entity->getComponent<components::RigidBody>();
				auto transform = entity->getComponent<components::Transform>();
				auto collider = entity->getComponent<components::Collider>();
				//if (transform->position.x + transform->scale.x < windowWidth)
				//{
				//	rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				//}
			}
				});




			auto mouseComponent = std::make_unique<components::MouseInput>();

			mouseComponent->bindings.insert({ SDL_BUTTON_LEFT, "mousePress" });
			mouseComponent->onPressActions.insert({ "mousePress", [=](entities::EntityPtr, Ebony::MousePress& mousePress) {std::cout << "Button pressed at " << mousePress.mouseClick.x << ", " << mousePress.mouseClick.y << std::endl; } });
			mouseComponent->onReleaseActions.insert({ "mousePress",[=](entities::EntityPtr, Ebony::MousePress&) {std::cout << "Mouse Button released!" << std::endl; } });

			//mouseComponent->loadMouseBindings("../mouseBindings.json");
			//mouseComponent->saveMouseBindings("../mouseBindings.json");
			player->addComponent(std::move(mouseComponent));

			player->addComponent(std::make_unique<components::Player>());

			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("paddle_0"), Ebony::Colors::White);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(50.0f, 50.0f), glm::vec2(100.0f, 100.0f), true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					Crypt::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					if (layer & Crypt::CollisionLayers::GROUND)
					{
						player->getComponent<components::Player>()->isOnGround = true;
					}
				};


			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::PLAYER | Crypt::CollisionLayers::GROUND, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(100.0f, 100.0f));
			
			// Will use a separate system for controlling gravity and such
			auto rigidbody = std::make_unique<components::RigidBody>(glm::vec2(100.0f, 500.0f), glm::vec2(0.0f, 200.0f));
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