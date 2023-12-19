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


namespace Crypt
{
	class Ground
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, int windowWidth)
		{
			entities::EntityPtr ground = std::make_shared<entities::Entity>();


			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::White);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(windowWidth / 2.0f, 50.0f), glm::vec2(windowWidth, 100.0f), true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::GROUND, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(windowWidth, 100.0f));
			auto rigidbody = std::make_unique<components::RigidBody>();



			ground->addComponent(std::move(collider));
			ground->addComponent(std::move(transform));
			ground->addComponent(std::move(sprite));
			ground->addComponent(std::move(rigidbody));

			return ground;
		}

	};
}