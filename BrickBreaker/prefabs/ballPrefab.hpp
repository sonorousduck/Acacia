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

			ballEntity->addComponent(std::move(std::make_unique<components::Transform>(beginningTransform, 0.0f, glm::vec2(20.0f, 20.0f))));
			auto spriteBall = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("ball"), Ebony::Colors::White);
			components::Subcollider ballAABBCollider = components::Subcollider(glm::vec2(10.0f, 10.0f), glm::vec2(20.0f, 20.0f), true, true);
			ballAABBCollider.onCollisionStart = [=](entities::EntityPtr self, entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					BrickBreaker::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

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
						glm::vec2 direction = self->getComponent<components::Ball>()->direction;
						self->getComponent<components::Ball>()->direction = glm::vec2(direction.x, -direction.y);
					}
					else if (layer & BrickBreaker::CollisionLayers::BRICK)
					{
						glm::vec2 direction = self->getComponent<components::Ball>()->direction;
						self->getComponent<components::Ball>()->direction = glm::vec2(direction.x, -direction.y);
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

			auto ballCollider = std::make_unique<components::Collider>(ballAABBCollider, BrickBreaker::CollisionLayers::ALL);
			auto ballComponent = std::make_unique<components::Ball>(200.0f, glm::vec2(0.5f, -0.5f), 1, ball, isStuckToPaddle);

			ballEntity->addComponent(std::move(ballCollider));
			ballEntity->addComponent(std::move(std::make_unique<components::RigidBody>()));
			ballEntity->addComponent(std::move(spriteBall));
			ballEntity->addComponent(std::move(ballComponent));

			return ballEntity;
		}
	};
}