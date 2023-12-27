#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>

namespace Crypt
{
	class PlayerHealth
	{
	public:
		static entities::EntityPtr Create(int windowWidth)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			entity->addComponent(std::make_unique<components::Text>("10/10", Ebony::Colors::White, Ebony::Colors::Black, Ebony::ResourceManager::GetFont("default"), 1.0f, true, true));
			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(static_cast<float>(windowWidth - 125), 25.0f), 0.0f, glm::vec2(100.0f, 100.0f)));
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::Green, 0.5f, true));



			return entity;
		}
	};
}