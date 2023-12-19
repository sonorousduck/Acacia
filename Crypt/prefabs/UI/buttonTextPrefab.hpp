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
		static entities::EntityPtr Create(float width, float height, const char* spriteText)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture(spriteText);


			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(width - (texture->Width / 3.0f), height), 0.0f, glm::vec2(texture->Width, texture->Height)));
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, 0.02f));



			return entity;
		}
	};
}