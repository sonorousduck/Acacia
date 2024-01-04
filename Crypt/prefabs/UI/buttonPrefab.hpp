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
#include <components/buttonComponent.hpp>
#include <components/activeUICollisionComponent.hpp>

namespace Crypt
{
	class Button
	{
	public:
		static entities::EntityPtr Create(float width, float height, const char* spriteImageUnpressed, const char* spriteImageHover, const char* spriteImagePressed, Crypt::ScreenEnum screenEnum, std::function<void(std::uint16_t)> setNextScreen)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture(spriteImageUnpressed);

			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(width, height), 0.0f, glm::vec2(texture->Width, texture->Height) * glm::vec2(0.5f, 0.5f)));

			components::Subcollider subcollider = components::Subcollider(glm::vec2(texture->Width / 2, texture->Height / 2) * glm::vec2(0.5f, 0.5f), glm::vec2(texture->Width, texture->Height) * glm::vec2(0.5f, 0.5f), true, true);
			

			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, 0.01f, true));
			

			auto button = std::make_unique<components::ButtonComponent>(spriteImageUnpressed, spriteImageHover, spriteImagePressed);

			button->onPress = [=](entities::EntityPtr self)
				{
					setNextScreen(screenEnum);
				};

			entity->addComponent(std::move(button));

			// Create the onCollisionStart
			subcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{

					components::ActiveUICollision* activeUICollision;
					if (other->tryGetComponent<components::ActiveUICollision>(activeUICollision))
					{
						entity->getComponent<components::Sprite>()->SetTexture(Ebony::ResourceManager::GetTexture(spriteImageHover));

						activeUICollision->collidingWith = entity;
					}

					//other->getComponent<components::
					// If it is clicked while onCollision though, I need to switch to the pressed version
				};
			
			// Create the onCollisionEnd
			subcollider.onCollisionEnd = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					entity->getComponent<components::Sprite>()->SetTexture(Ebony::ResourceManager::GetTexture(spriteImageUnpressed));

					if (other->getComponent<components::ActiveUICollision>()->collidingWith == entity)
					{
						other->getComponent<components::ActiveUICollision>()->collidingWith = nullptr;
					}
				};

			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::UI, CollisionLayers::UI, true));
			entity->addComponent(std::make_unique<components::RigidBody>());



			return entity;
		}
	};
}