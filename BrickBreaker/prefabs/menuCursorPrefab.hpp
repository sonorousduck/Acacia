#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/collider.hpp>
#include <components/subcolliderClass.hpp>
#include <components/rigidbodyComponent.hpp>
#include "../misc/collisionLayers.hpp"
#include <iostream>

namespace BrickBreaker
{
	class MenuCursor
	{
	public:
		static entities::EntityPtr Create()
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			

			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(50.0f, 50.0f)));

			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("heart"), Ebony::Colors::Black, 0.001f));

			components::Subcollider subcollider = components::Subcollider(glm::vec2(0.5f, 0.5f), glm::vec2(50.0f, 50.0f), true, true);
			entity->addComponent(std::make_unique<components::RigidBody>());


			auto mouseInput = std::make_unique<components::MouseInput>();

			mouseInput->onMove = [=](entities::EntityPtr self)
				{
					auto mouseInput = self->getComponent<components::MouseInput>();

					self->getComponent<components::Transform>()->position = glm::vec2(mouseInput->positionX, mouseInput->positionY);
				};
			
			entity->addComponent(std::move(mouseInput));
			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::UI, true, false));
			entity->addComponent(std::make_unique<components::RigidBody>());



			return entity;
		}
	};
}