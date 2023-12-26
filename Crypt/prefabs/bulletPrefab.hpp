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
#include <components/destructionComponent.hpp>
#include <components/timedComponent.hpp>

#include <singletons/time.hpp>
#include <components/mouseInputComponent.hpp>
#include <components/animationControllerComponent.hpp>
#include "../components/bulletComponent.hpp"
#include "../components/aimComponent.hpp"
#include <iostream>

namespace Crypt
{
	class BulletPrefab
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, glm::vec2 direction, float speed, components::BULLET_TYPE bulletType, std::uint8_t strength, const char* texture)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(texture), Ebony::Colors::White, 0.11f);
			components::Subcollider aabbcollider = components::Subcollider(scale / 2.0f, scale, true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::GROUND, false);
			float test = (glm::atan(direction.y, direction.x)) / (2 * glm::pi<float>()) * 360.0f;
			std::cout << test << std::endl;
			auto transform = std::make_unique<components::Transform>(startTransform, (glm::atan(direction.y, direction.x)) / (2 * glm::pi<float>()) * 360.0f, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();
			rigidbody->setVelocity(direction * speed);



			entity->addComponent(std::make_unique<components::TimedComponent>(1.0f, [=]() 
				{
					entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;				
				}));
			entity->addComponent(std::make_unique<components::DestructionComponent>([=]() 
				{
					entity->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));


			entity->addComponent(std::make_unique<components::Bullet>(bulletType, strength));
			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(rigidbody));

			return entity;
		}

	};
}