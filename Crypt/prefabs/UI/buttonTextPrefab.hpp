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
#include <memory>

namespace Crypt
{
	class ButtonText
	{
	public:
		static entities::EntityPtr Create(float width, float height, float buttonWidth, float buttonHeight, const char* spriteText)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture(spriteText);
			auto scale = glm::vec2(texture->Width, texture->Height) * glm::vec2(0.5f, 0.5f);

			auto centeredPositionX = (buttonWidth - scale.x) / 2.0f;
			auto centeredPositionY = (buttonHeight - scale.y) / 2.0f;



			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.


			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(centeredPositionX + width, height + centeredPositionY), 0.0f, scale));
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, 0.02f, true));



			return entity;
		}
	};
}