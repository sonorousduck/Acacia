#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>
#include <misc/renderLayers.hpp>

namespace Crypt
{
	class Heart
	{
	public:
		static entities::EntityPtr Create(glm::vec2 position)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("full_heart"), Ebony::Colors::White, Ebony::RenderLayer::ALWAYS_FRONT, true);
			auto scale = sprite->GetDimensions();

			entity->addComponent(std::move(std::make_unique<components::Transform>(position, 0.0f, scale)));
			entity->addComponent(std::move(sprite));


			return entity;
		}
	};
}