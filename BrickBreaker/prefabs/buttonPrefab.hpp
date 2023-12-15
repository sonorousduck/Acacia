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
	class Button
	{
	public:
		static entities::EntityPtr Create(float width, float height, const char* spriteImageUnpressed, const char* spriteImageHover, const char* spriteImagePressed)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture(spriteImageUnpressed);

			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(width, height), 0.0f, glm::vec2(texture->Width, texture->Height)));

			components::Subcollider subcollider = components::Subcollider(glm::vec2(texture->Width / 2, texture->Height / 2), glm::vec2(texture->Width, texture->Height), true, true);
			

			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, 0.01f));


			// Create the onCollisionStart
			subcollider.onCollisionStart = [=](entities::EntityPtr self, entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					self->getComponent<components::Sprite>()->SetTexture(Ebony::ResourceManager::GetTexture(spriteImageHover));
					// If it is clicked while onCollision though, I need to switch to the pressed version
				};
			
			// Create the onCollisionEnd
			subcollider.onCollisionEnd = [=](entities::EntityPtr self, entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					self->getComponent<components::Sprite>()->SetTexture(Ebony::ResourceManager::GetTexture(spriteImageUnpressed));
				};

			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::UI, true, false));
			entity->addComponent(std::make_unique<components::RigidBody>());



			return entity;
		}
	};
}