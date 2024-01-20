#pragma once
#include <components/mouseInputComponent.hpp>
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/collider.hpp>
#include <components/subcolliderClass.hpp>
#include <components/rigidbodyComponent.hpp>
#include "../misc/collisionLayers.hpp"
#include <iostream>
#include <components/activeUICollisionComponent.hpp>
#include <components/buttonComponent.hpp>

namespace EcologyRTS
{
	class MainGameCursor
	{
	public:
		static entities::EntityPtr Create()
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("cursor"), Ebony::Colors::White, Ebony::RenderLayer::ALWAYS_FRONT, true);
			auto scale = sprite->GetDimensions();
			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(0.0f, 0.0f), 0.0f, scale));
			components::Subcollider subcollider = components::Subcollider(glm::vec2(0.5f, 0.5f), glm::vec2(1.0f, 1.0f), true, true);
			entity->addComponent(std::make_unique<components::RigidBody>());


			auto mouseInput = std::make_unique<components::MouseInput>();

			mouseInput->onMove = [=]()
				{
					auto mouseInput = entity->getComponent<components::MouseInput>();

					entity->getComponent<components::Transform>()->position = glm::vec2(mouseInput->positionX, mouseInput->positionY);
				};

			mouseInput->bindings.insert({ SDL_BUTTON_LEFT, "pressButton" });


			mouseInput->onPressActions.insert({ "pressButton", [=](Ebony::MousePress& mousePress)
				{
					


				} });


			entity->addComponent(std::move(mouseInput));
			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::UI | CollisionLayers::GROUND, false));
			entity->addComponent(std::make_unique<components::RigidBody>());
			entity->addComponent(std::move(sprite));



			return entity;
		}
	};
}