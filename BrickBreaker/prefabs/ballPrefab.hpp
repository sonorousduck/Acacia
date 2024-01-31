#pragma once
#include <components/controllerComponent.hpp>
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include "../components/ballComponent.hpp"
#include <components/rigidbodyComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <singletons/inputManager.hpp>
#include <components/soundEffectComponent.hpp>
#include <misc/renderLayers.hpp>

#include <singletons/systemManager.hpp>
#include "../screens/screenEnums.hpp"
#include "../scripts/DeathScript.hpp"
#include <components/aiComponent.hpp>
#include "../misc/aiInformationTypes.hpp"
#include <components/aiInputComponent.hpp>

#include "../singletons/PythonManager.hpp"

namespace BrickBreaker
{
	class Ball
	{
	public:
		static entities::EntityPtr Create(glm::vec2 beginningTransform, entities::EntityPtr ball = nullptr, bool isStuckToPaddle = false)
		{
			entities::EntityPtr ballEntity = std::make_shared<entities::Entity>();

			ballEntity->addComponent(std::make_unique<components::SoundEffect>(Ebony::ENTITY));

			auto spriteBall = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("ball"), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto scale = spriteBall->GetDimensions();

			ballEntity->addComponent(std::move(std::make_unique<components::Transform>(beginningTransform, 0.0f, scale)));
			components::Subcollider ballAABBCollider = components::Subcollider(scale * 0.5f, scale, true, true);
			
			ballAABBCollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					BrickBreaker::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					ballEntity->getComponent<components::SoundEffect>()->soundEffectQueue.push_back(Ebony::IndividualSound(Ebony::ResourceManager::GetSoundEffect("ball_bounce"), 50));

					if (layer & BrickBreaker::CollisionLayers::WALL | layer & BrickBreaker::CollisionLayers::BRICK)
					{
						auto transform = ballEntity->getComponent<components::Transform>();
						auto wallTransform = other->getComponent<components::Transform>();
						glm::vec2 directionVector = glm::normalize(transform->position - transform->previousPosition);
						auto ball = ballEntity->getComponent<components::Ball>();
						auto collisionLocation = ballEntity->getComponent<components::Collider>()->aabbCollider.lastCollisionOnOtherObjectLocation;


						// If hit the top/bottom
						if (collisionLocation.y >= wallTransform->position.y + wallTransform->scale.y || collisionLocation.y <= wallTransform->position.y)
						{
							directionVector.y = -directionVector.y;
						}

						// If it hit the side
						else if (collisionLocation.x >= wallTransform->position.x + wallTransform->scale.x || collisionLocation.x <= wallTransform->position.x)
						{
							directionVector.x = -directionVector.x;

						}

						if (glm::abs(directionVector.y) < 0.05)
						{
							if (directionVector.y < 0) directionVector.y -= 0.2;
							else directionVector.y += 0.2;
							if (directionVector.x > 0) directionVector.x -= 0.2;
							else directionVector.x += 0.2;


						}


						ball->direction = directionVector;

					}
					
					else if (layer & BrickBreaker::CollisionLayers::BOTTOM_WALL)
					{
						if (Ebony::SystemManager::aiEnabled)
						{
							Ebony::SystemManager::shouldResetForAi = true;
						}
						else
						{
							Ebony::SystemManager::nextScreenEnum = BrickBreaker::ScreenEnum::MAIN_MENU;
						}

					}
					else if (layer & BrickBreaker::CollisionLayers::PADDLE)
					{
						components::Transform* paddleTransform = other->getComponent<components::Transform>();
						components::Ball* ball = ballEntity->getComponent<components::Ball>();
						// Collision with paddle should take into account a bit of randomness, velocity at which the paddle is moving, and where it hit on the paddle
						glm::vec2 direction = ballEntity->getComponent<components::Transform>()->position - paddleTransform->position;
						glm::vec2 bounceDirection = glm::abs(ball->direction);
						glm::vec2 paddleScale = paddleTransform->scale;



						float halfpoint = paddleScale.x / 2.0f;
						float oneTenth = paddleScale.x / 10.0f;

						bool isMiddlePaddle = direction.x >= halfpoint - 1.25 * oneTenth && direction.x <= halfpoint + 1.25 * oneTenth;
						

						if (isMiddlePaddle)
						{
							bounceDirection.x = static_cast<float>(ball->random_double(-0.5, 0.5));
							//Ebony::InputManager::controllerInstances[Ebony::InputManager::sdlJoystickToJoystickConversion[self->getComponent<components::ControllerInput>()->joystickId]]->Vibrate(1, 1, 100, false);
							Ebony::InputManager::Vibrate(ballEntity->getComponent<components::ControllerInput>()->joystickId, 1, 1, 100, false);

						}
						else
						{
							// Check right side
							if (direction.x >= halfpoint)
							{
								if (direction.x <= paddleScale.x - 1.5 * oneTenth) // Just right side
								{
									Ebony::InputManager::Vibrate(ballEntity->getComponent<components::ControllerInput>()->joystickId, 0, 0.5, 100, false);
									//Ebony::InputManager::controllerInstances[Ebony::InputManager::sdlJoystickToJoystickConversion[self->getComponent<components::ControllerInput>()->joystickId]]->Vibrate(0, 0.5, 100, false);
									bounceDirection.x = 1.25;
								}
								else // Right Edge
								{
									Ebony::InputManager::Vibrate(ballEntity->getComponent<components::ControllerInput>()->joystickId, 0, 1, 100, true);

									//Ebony::InputManager::controllerInstances[Ebony::InputManager::sdlJoystickToJoystickConversion[self->getComponent<components::ControllerInput>()->joystickId]]->Vibrate(0, 1, 100, true);
									bounceDirection.x = 1.75;
								}
							}
							else
							{
								if (direction.x >= 1.5 * oneTenth) // Left Side
								{
									Ebony::InputManager::Vibrate(ballEntity->getComponent<components::ControllerInput>()->joystickId, 0.5, 0, 100, false);

									//Ebony::InputManager::controllerInstances[Ebony::InputManager::sdlJoystickToJoystickConversion[self->getComponent<components::ControllerInput>()->joystickId]]->Vibrate(0.5, 0, 100, false);
									bounceDirection.x = -1.25;
								}
								else // Left Edge
								{
									Ebony::InputManager::Vibrate(ballEntity->getComponent<components::ControllerInput>()->joystickId, 1, 0, 100, true);

									//Ebony::InputManager::controllerInstances[Ebony::InputManager::sdlJoystickToJoystickConversion[self->getComponent<components::ControllerInput>()->joystickId]]->Vibrate(1, 0, 100, true);
									bounceDirection.x = -1.75;
								}
							}	
						}

						bounceDirection.y = -bounceDirection.y;

						//glm::vec2 direction = self->getComponent<components::Ball>()->direction;
						ballEntity->getComponent<components::Ball>()->direction = glm::normalize(bounceDirection);
					}	
				};

				ballAABBCollider.onCollision = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
					{
						BrickBreaker::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);
						if (layer & BrickBreaker::CollisionLayers::BOTTOM_WALL)
						{
							if (Ebony::SystemManager::aiEnabled)
							{
								Ebony::SystemManager::shouldResetForAi = true;
							}
							else
							{
								Ebony::SystemManager::nextScreenEnum = BrickBreaker::ScreenEnum::MAIN_MENU;
							}
						}
					};
				
				ballAABBCollider.onCollisionEnd = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
					{
						BrickBreaker::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

						if (layer & BrickBreaker::CollisionLayers::BOTTOM_WALL)
						{
							if (Ebony::SystemManager::aiEnabled)
							{
								Ebony::SystemManager::shouldResetForAi = true;
							}
							else
							{
								Ebony::SystemManager::nextScreenEnum = BrickBreaker::ScreenEnum::MAIN_MENU;
							}
						}
						else if (layer & BrickBreaker::CollisionLayers::BRICK)
						{
							auto brick = other->getComponent<components::Brick>();
							brick->hitAlready = false;
						}
					};



			std::unique_ptr<components::KeyboardInput> keyboardInputComponentBall = std::make_unique<components::KeyboardInput>();
			std::unique_ptr<components::ControllerInput> controllerComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::AiInput> aiInput = std::make_unique<components::AiInput>();


			keyboardInputComponentBall->bindings.insert({ SDLK_SPACE, "launchBall" });

			keyboardInputComponentBall->onReleaseActions.insert({ "launchBall", [=]()
			{
				//TODO: Make this only happen when isAttachedToPaddle is true. Right now, it is nice for debugging though :)
				auto ball = ballEntity->getComponent<components::Ball>();
				ball->isAttachedToPaddle = false;
				double random_x = ball->random_double(-0.8, 0.8);
				double random_y = ball->random_double(-0.8, 0.8);
				ball->direction = glm::normalize(glm::vec2(random_x, -abs(random_y)));
			} });

			controllerComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_A, "launchBall" });
			controllerComponent->onReleaseActions.insert({ "launchBall", [=]()
			{
					//TODO: Make this only happen when isAttachedToPaddle is true. Right now, it is nice for debugging though :)
					auto ball = ballEntity->getComponent<components::Ball>();
					ball->isAttachedToPaddle = false;
					double random_x = ball->random_double(-0.8, 0.8);
					double random_y = ball->random_double(-0.8, 0.8);
					ball->direction = glm::normalize(glm::vec2(random_x, -abs(random_y)));
				} });

			ballEntity->addComponent(std::move(keyboardInputComponentBall));
			ballEntity->addComponent(std::move(controllerComponent));


			aiInput->actions.insert({ "launchBall", [=]()
			{
					//TODO: Make this only happen when isAttachedToPaddle is true. Right now, it is nice for debugging though :)
					auto ball = ballEntity->getComponent<components::Ball>();
					
					if (ball->isAttachedToPaddle)
					{
						ball->isAttachedToPaddle = false;
						double random_x = ball->random_double(-0.6, 0.6);
						double random_y = ball->random_double(-0.6, 0.6);
						ball->direction = glm::normalize(glm::vec2(random_x, -abs(random_y)));
					}

				} });


			aiInput->translationFunction = [=]()
				{
					auto& action = BrickBreaker::PythonManager::action;

					if (action == 2)
					{
						ballEntity->getComponent<components::AiInput>()->actions["launchBall"]();
					}
				};



			auto ballCollider = std::make_unique<components::Collider>(ballAABBCollider, BrickBreaker::CollisionLayers::BALL, BrickBreaker::CollisionLayers::PADDLE | BrickBreaker::CollisionLayers::BRICK | BrickBreaker::CollisionLayers::WALL | BrickBreaker::CollisionLayers::BOTTOM_WALL, false);
			auto ballComponent = std::make_unique<components::Ball>(200.0f, glm::vec2(0.5f, -0.5f), 1, ball, isStuckToPaddle);


			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::DeathScript>();

			ballEntity->addComponent(std::move(script));
			ballEntity->addComponent(std::move(aiInput));
			ballEntity->addComponent(std::move(ballCollider));
			ballEntity->addComponent(std::move(std::make_unique<components::RigidBody>()));
			ballEntity->addComponent(std::move(spriteBall));
			ballEntity->addComponent(std::move(ballComponent));
			ballEntity->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE, BrickBreaker::AiInformationTypes::BALL_INFORMATION));


			return ballEntity;
		}
	};
}