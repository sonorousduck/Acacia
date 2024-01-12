#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <misc/renderLayers.hpp>

namespace Crypt
{
	class CryptTitle
	{
	public:
		static entities::EntityPtr Create(int windowWidth)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture("crypt_title");
			auto scale = glm::vec2(texture->Width, texture->Height);

			auto centeredPositionX = (static_cast<float>(windowWidth) - scale.x) / 2.0f;


			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.
			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(centeredPositionX, 200.0f), 0.0f, scale));
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, Ebony::RenderLayer::UI_RENDER, true));


			return entity;
		}
	};
}