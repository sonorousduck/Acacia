#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <memory>
#include <misc/renderLayers.hpp>

namespace EcologyRTS
{
	class StoreInventory
	{
	public:
		static entities::EntityPtr Create()
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture("store_frame");
			auto scale = glm::vec2(texture->Width, texture->Height);


			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.


			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, Ebony::RenderLayer::UI_RENDER, true));
			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(260.0f, 22.0f), 0.0f, scale));


			return entity;
		}
	};
}