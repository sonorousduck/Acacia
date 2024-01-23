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
#include <components/activeUICollisionComponent.hpp>
#include <components/buttonComponent.hpp>


namespace SpaceGuy
{
	class MenuCursor
	{
	public:
		static entities::EntityPtr Create()
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			

			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f)));
			entity->addComponent(std::make_unique<components::ActiveUICollision>());
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
					auto activeCollisionComponent = entity->getComponent<components::ActiveUICollision>();

					if (activeCollisionComponent->collidingWith != nullptr)
					{
						activeCollisionComponent->collidingWith->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture(activeCollisionComponent->collidingWith->getComponent<components::ButtonComponent>()->spriteImagePressed);

						if (activeCollisionComponent->collidingWith->getComponent<components::ButtonComponent>()->onPress.has_value())
						{
							activeCollisionComponent->collidingWith->getComponent<components::ButtonComponent>()->onPress.value()(activeCollisionComponent->collidingWith);
						}
					}


				}});


			entity->addComponent(std::move(mouseInput));
			entity->addComponent(std::make_unique<components::Collider>(subcollider, SpaceGuy::CollisionLayers::UI, false));
			entity->addComponent(std::make_unique<components::RigidBody>());


			return entity;
		}
	};
}