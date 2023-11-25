#pragma once
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
#include <window.hpp>

namespace BrickBreaker
{
	class Ball
	{
	public:
		static entities::EntityPtr Create(glm::vec2 beginningTransform, entities::EntityPtr ball = nullptr, bool isStuckToPaddle = false)
		{
			entities::EntityPtr ballEntity = std::make_shared<entities::Entity>();

			ballEntity->addComponent(std::move(std::make_unique<components::SoundEffect>(EbonyAudio::ENTITY)));


			ballEntity->addComponent(std::move(std::make_unique<components::Transform>(beginningTransform, 0.0f, glm::vec2(20.0f, 20.0f))));
			auto spriteBall = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("ball"), Ebony::Colors::White);
			components::Subcollider ballAABBCollider = components::Subcollider(glm::vec2(10.0f, 10.0f), glm::vec2(20.0f, 20.0f), true, true);
			ballAABBCollider.onCollisionStart = [=](entities::EntityPtr self, entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					BrickBreaker::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					ballEntity->getComponent<components::SoundEffect>()->soundEffectQueue.push_back(Ebony::ResourceManager::GetSoundEffect("ball_bounce"));

					if (layer & BrickBreaker::CollisionLayers::WALL)
					{
						glm::vec2 direction = self->getComponent<components::Ball>()->direction;
						self->getComponent<components::Ball>()->direction = glm::vec2(-direction.x, direction.y);
					}
					else if (layer & BrickBreaker::CollisionLayers::TOP_WALL)
					{
						glm::vec2 direction = self->getComponent<components::Ball>()->direction;
						self->getComponent<components::Ball>()->direction = glm::vec2(direction.x, -direction.y);
					}
					else if (layer & BrickBreaker::CollisionLayers::PADDLE)
					{
						components::Transform* paddleTransform = other->getComponent<components::Transform>();
						components::Ball* ball = self->getComponent<components::Ball>();
						// Collision with paddle should take into account a bit of randomness, velocity at which the paddle is moving, and where it hit on the paddle
						glm::vec2 direction = self->getComponent<components::Transform>()->position - paddleTransform->position;
						glm::vec2 bounceDirection = glm::abs(ball->direction);
						glm::vec2 paddleScale = paddleTransform->scale;

						float halfpoint = paddleScale.x / 2.0f;
						float oneTenth = paddleScale.x / 10.0f;

						bool isMiddlePaddle = direction.x >= halfpoint - 1.25 * oneTenth && direction.x <= halfpoint + 1.25 * oneTenth;


						if (isMiddlePaddle)
						{
							bounceDirection.x = static_cast<float>(ball->random_double(-0.5, 0.5));
							
						}
						else
						{
							// Check right side
							if (direction.x >= halfpoint)
							{
								if (direction.x <= paddleScale.x - 1.5 * oneTenth) // Just right side
								{
									bounceDirection.x = 1.25;
								}
								else // Right Edge
								{
									bounceDirection.x = 1.75;

								}
							}
							else
							{
								if (direction.x >= 1.5 * oneTenth) // Left Side
								{
									bounceDirection.x = -1.25;
								}
								else // Left Edge
								{
									bounceDirection.x = -1.75;
								}
							}	
						}

						bounceDirection.y = -bounceDirection.y;

						//glm::vec2 direction = self->getComponent<components::Ball>()->direction;
						self->getComponent<components::Ball>()->direction = bounceDirection;
					}
					else if (layer & BrickBreaker::CollisionLayers::BRICK)
					{
						// Much more intricate collision system is needed.
						// First, subtracting the one position from the other should give us everything we need
						// If direction.y > 0 -> hit the bottom, else top
						// If direction.x > 0 -> hit the right, else left
						if (!self->getComponent<components::Ball>()->directionChangedThisFrame)
						{



							glm::vec2 direction = self->getComponent<components::Transform>()->position - other->getComponent<components::Transform>()->position;
							glm::vec2 bounceDirection = self->getComponent<components::Ball>()->direction;
							// We need to detect whether it hit the left or the right side
							// Then detect whether it hit the top or the bottom side

							// We hit "directly" the side, handle this specifically



							if (direction.y <= 7 && direction.y >= -7)
							{
								std::cout << "This happened" << std::endl;
								bounceDirection.x = -bounceDirection.x;

							}
							else
							{
								bounceDirection.y = -bounceDirection.y;
							}




							// If it hit the right side, then the x velocity should negative
							//if (direction.x > 0)
							//{
							//	bounceDirection.x = -bounceDirection.x;
							//}
							// If ball hit the bottom size, y velocity should be negative


							//self->getComponent<components::Ball>()->SetNewDirection(bounceDirection);

							self->getComponent<components::Ball>()->direction = bounceDirection;
							self->getComponent<components::Ball>()->directionChangedThisFrame = true;
						}


						
					}
				};

			std::unique_ptr<components::KeyboardInput> keyboardInputComponentBall = std::make_unique<components::KeyboardInput>();
			
			keyboardInputComponentBall->bindings.insert({ GLFW_KEY_SPACE, "launchBall" });

			keyboardInputComponentBall->onReleaseActions.insert({ "launchBall", [=](entities::EntityPtr entity)
			{
				//TODO: Make this only happen when isAttachedToPaddle is true. Right now, it is nice for debugging though :)
				auto ball = entity->getComponent<components::Ball>();
				ball->isAttachedToPaddle = false;
				double random_x = ball->random_double(-0.8, 0.8);
				double random_y = ball->random_double(-0.8, 0.8);
				ball->direction = glm::vec2(random_x, -abs(random_y));
			} });

			ballEntity->addComponent(std::move(keyboardInputComponentBall));

			auto ballCollider = std::make_unique<components::Collider>(ballAABBCollider, BrickBreaker::CollisionLayers::PADDLE | BrickBreaker::CollisionLayers::BRICK | BrickBreaker::CollisionLayers::WALL | BrickBreaker::CollisionLayers::TOP_WALL, false);
			auto ballComponent = std::make_unique<components::Ball>(600.0f, glm::vec2(0.5f, -0.5f), 1, ball, isStuckToPaddle);

			ballEntity->addComponent(std::move(ballCollider));
			ballEntity->addComponent(std::move(std::make_unique<components::RigidBody>()));
			ballEntity->addComponent(std::move(spriteBall));
			ballEntity->addComponent(std::move(ballComponent));

			return ballEntity;
		}
	};
}