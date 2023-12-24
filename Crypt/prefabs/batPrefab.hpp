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
#include "../components/shootingComponent.hpp"

namespace Crypt
{
	class Bat
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, entities::EntityPtr player)
		{
			entities::EntityPtr bat = std::make_shared<entities::Entity>();

			float detectionRange = 10.0f;

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("bat"), Ebony::Colors::White, 0.11);
			components::Subcollider aabbcollider = components::Subcollider(scale / glm::vec2(2.0f, 2.0f), scale, true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::ENEMY, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();


			auto shootingComponent = std::make_unique<components::Shooting>();

			bat->addComponent(std::move(collider));
			bat->addComponent(std::move(transform));
			bat->addComponent(std::move(sprite));
			bat->addComponent(std::move(rigidbody));

			return bat;
		}

	};
}