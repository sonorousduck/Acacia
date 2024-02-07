#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>

namespace Crypt
{
	class GameComplete
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, float windowHeight)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::White, Ebony::RenderLayer::ALWAYS_FRONT);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(5.0f, windowHeight / 2.0f), glm::vec2(10.0f, windowHeight), true, true);
			
			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::PLAYER)
					{
						Ebony::SystemManager::goodEnding = true;

						if (Ebony::SystemManager::aiEnabled)
						{
							Ebony::SystemManager::shouldResetForAi = true;
							Ebony::SystemManager::goodEnding = true;
						}
						else
						{
							Ebony::SystemManager::nextScreenEnum = Crypt::ScreenEnum::GAME_OVER;
						}
					}
				};
			
			
			
			
			auto collider = std::make_unique<components::Collider>(aabbcollider, CollisionLayers::ENDING_WALL, CollisionLayers::PLAYER, true);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(5.0F, windowHeight));
			auto rigidbody = std::make_unique<components::RigidBody>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::Red, Ebony::ALWAYS_FRONT);






			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(transform));
			//ground->addComponent(std::move(sprite));
			entity->addComponent(std::move(rigidbody));
			entity->addComponent(std::move(sprite));

			return entity;
		}

	};
}