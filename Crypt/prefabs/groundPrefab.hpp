#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>

namespace Crypt
{
	class Ground
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, float windowWidth)
		{
			entities::EntityPtr ground = std::make_shared<entities::Entity>();


			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::White, Ebony::RenderLayer::ALWAYS_FRONT);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(windowWidth / 2.0f, 5.0f), glm::vec2(windowWidth, 10.0f), true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, CollisionLayers::GROUND, CollisionLayers::ENEMY_BULLET | CollisionLayers::PLAYER_BULLET | CollisionLayers::ENEMY | CollisionLayers::PLAYER, true);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(windowWidth, 10.0f));
			auto rigidbody = std::make_unique<components::RigidBody>();



			ground->addComponent(std::move(collider));
			ground->addComponent(std::move(transform));
			//ground->addComponent(std::move(sprite));
			ground->addComponent(std::move(rigidbody));

			return ground;
		}

	};
}