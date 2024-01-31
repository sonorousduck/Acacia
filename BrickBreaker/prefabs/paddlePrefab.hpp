#pragma once
#include <components/rigidbodyComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <components/controllerComponent.hpp>
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <graphics2d.hpp>
#include <SDL.h>
#include <singletons/time.hpp>
#include <components/soundEffectComponent.hpp>
#include <components/mouseInputComponent.hpp>
#include <misc/renderLayers.hpp>

#include <components/timedComponent.hpp>
#include <components/aiComponent.hpp>
#include "../misc/aiInformationTypes.hpp"
#include <components/aiInputComponent.hpp>
#include "../singletons/PythonManager.hpp"

namespace BrickBreaker
{
	class Paddle
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, int windowWidth)
		{
			entities::EntityPtr paddle = std::make_shared<entities::Entity>();
			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("paddle_0"), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto scale = sprite->GetDimensions();
			auto paddleSpeed = 350.0f;

			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();
			std::unique_ptr<components::AiInput> aiInput = std::make_unique<components::AiInput>();

			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_B, "print" });
			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_Y, "printRelease" });


			controllerInputComponent->onHeldActions.insert({ "print", [=]() {std::cout << "Circle was called (OnHeld)" << std::endl; } });
			controllerInputComponent->onReleaseActions.insert({ "printRelease", [=]() {std::cout << "Triangle was called" << std::endl; } });
			controllerInputComponent->joystickBindings.insert({ SDL_CONTROLLER_AXIS_LEFTX, "paddleMovement" });

			controllerInputComponent->joystickActions.insert({ "paddleMovement", [=](float value) {
				if (abs(value) > 0.10)
				{
					auto rigidBody = paddle->getComponent<components::RigidBody>();
					auto transform = paddle->getComponent<components::Transform>();
					auto collider = paddle->getComponent<components::Collider>();

					if (value < 0 && transform->position.x > 0)
					{
						std::cout << Ebony::Time::GetDeltaTimeFloat() << std::endl;
						rigidBody->addScriptedMovement(glm::vec2{ paddleSpeed * Ebony::Time::GetDeltaTimeFloat() * value, 0.0f });
					}
					else if (value > 0 && transform->position.x + transform->scale.x < windowWidth)
					{
						rigidBody->addScriptedMovement(glm::vec2{ paddleSpeed * Ebony::Time::GetDeltaTimeFloat() * value, 0.0f });
					}
				}
			} });
		
			keyboardInputComponent->onReleaseActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->onHeldActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->bindings.insert({ SDLK_a, "paddleLeft" });
			keyboardInputComponent->bindings.insert({ SDLK_d, "paddleRight" });
			keyboardInputComponent->bindings.insert({ SDLK_e, "growShrink" });

			keyboardInputComponent->onHeldActions.insert({ "paddleLeft", [=]()
			{
				auto rigidBody = paddle->getComponent<components::RigidBody>();
				auto transform = paddle->getComponent<components::Transform>();
				auto collider = paddle->getComponent<components::Collider>();


				if (transform->position.x > 0)
				{
					rigidBody->addScriptedMovement(glm::vec2{ -paddleSpeed * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				}
			} });

			keyboardInputComponent->onHeldActions.insert({ "paddleRight", [=]()
			{
				auto rigidBody = paddle->getComponent<components::RigidBody>();
				auto transform = paddle->getComponent<components::Transform>();
				auto collider = paddle->getComponent<components::Collider>();
				if (transform->position.x + transform->scale.x < windowWidth)
				{
					rigidBody->addScriptedMovement(glm::vec2{ paddleSpeed * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				}
			}
				});


			keyboardInputComponent->onPressActions.insert({ "growShrink", [=]()
			{
				auto transform = paddle->getComponent<components::Transform>();

				if (transform->scale.x == 48.0f)
				{
					transform->scale.x += 16.0f;
				}
				else
				{
					transform->scale.x = 48.0f;
				}
			} });



			auto mouseComponent = std::make_unique<components::MouseInput>();

			mouseComponent->bindings.insert({SDL_BUTTON_LEFT, "mousePress" });
			mouseComponent->onPressActions.insert({ "mousePress", [=](Ebony::MousePress& mousePress) {std::cout << "Button pressed at " << mousePress.mouseClick.x << ", " << mousePress.mouseClick.y << std::endl; }});
			mouseComponent->onReleaseActions.insert({ "mousePress",[=](Ebony::MousePress&) {std::cout << "Mouse Button released!" << std::endl; } });

			//mouseComponent->loadMouseBindings("../mouseBindings.json");
			//mouseComponent->saveMouseBindings("../mouseBindings.json");
			paddle->addComponent(std::move(mouseComponent));

			paddle->addComponent(std::make_unique<components::SoundEffect>(Ebony::ENTITY));


			components::Subcollider aabbcollider = components::Subcollider(scale / 2.0f, scale, true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, BrickBreaker::CollisionLayers::PADDLE, BrickBreaker::CollisionLayers::POWERUP | BrickBreaker::CollisionLayers::BALL, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();

			auto largePaddleTime = std::make_unique<components::TimedComponent>(10.0f, [=]() {
				auto transform = paddle->getComponent<components::Transform>();
				auto collider = paddle->getComponent<components::Collider>();
				
				if (transform->scale.x == 64.0f)
				{
					transform->scale.x -= 16.0f;

					collider->aabbCollider.changeCenter(collider->aabbCollider.getCenter() - glm::vec2(8.0f, 0.0f));
					collider->aabbCollider.changeSize(collider->aabbCollider.getSize() - glm::vec2(16.0f, 0.0f));
				}});


			aiInput->actions.insert({ "paddleLeft", [=]()
			{
				auto rigidBody = paddle->getComponent<components::RigidBody>();
				auto transform = paddle->getComponent<components::Transform>();
				auto collider = paddle->getComponent<components::Collider>();


				if (transform->position.x > 0)
				{
					rigidBody->addScriptedMovement(glm::vec2{ -paddleSpeed * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				}
			} });

			aiInput->actions.insert({ "paddleRight", [=]()
			{
				auto rigidBody = paddle->getComponent<components::RigidBody>();
				auto transform = paddle->getComponent<components::Transform>();
				auto collider = paddle->getComponent<components::Collider>();
				if (transform->position.x + transform->scale.x < windowWidth)
				{
					rigidBody->addScriptedMovement(glm::vec2{ paddleSpeed * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				}
			}
				});


			aiInput->translationFunction = [=]()
				{
					auto& action = BrickBreaker::PythonManager::action;

					if (action == 0)
					{
						paddle->getComponent<components::AiInput>()->actions["paddleLeft"]();
					}
					if (action == 1)
					{
						paddle->getComponent<components::AiInput>()->actions["paddleRight"]();
					}
				};


			paddle->addComponent(std::move(collider));
			paddle->addComponent(std::move(largePaddleTime));
			paddle->addComponent(std::move(transform));
			paddle->addComponent(std::move(sprite));
			paddle->addComponent(std::move(keyboardInputComponent));
			paddle->addComponent(std::move(controllerInputComponent));
			paddle->addComponent(std::move(rigidbody));
			paddle->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE | Ebony::AIType::REWARD, BrickBreaker::AiInformationTypes::PADDLE_INFORMATION));
			paddle->addComponent(std::move(aiInput));

		
			return paddle;
		}

	};
}