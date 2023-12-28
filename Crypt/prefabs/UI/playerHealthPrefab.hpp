#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>
#include <iostream>

namespace Crypt
{
	class PlayerHealth
	{
	public:
		static entities::EntityPtr Create(int windowWidth)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			std::cout << windowWidth << std::endl;

			auto textComponent = std::make_unique<components::Text>("10/10", Ebony::Colors::Black, Ebony::Colors::White, Ebony::ResourceManager::GetFont("evil_empire"), 0.15f, true, true, glm::vec2(1.0f, 1.0f));
			//entity->addComponent(std::make_unique<components::Transform>(glm::vec2(static_cast<float>(windowWidth - 125), 25.0f), 0.0f, glm::vec2(100.0f, 50.0f)));
			entity->addComponent(std::move(std::make_unique<components::Transform>(glm::vec2{ 300.0f, 100.0f }, 0.0f, glm::vec2{ 100.0f, 50.0f })));
			//entity->addComponent(std::move(std::make_unique<components::Transform>(glm::vec2{ static_cast<float>(windowWidth) - 400.0f, 25.0f }, 0.0f, glm::vec2(100.0f, 50.0f))));

			entity->addComponent(std::move(textComponent));
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Color(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, true));



			return entity;
		}
	};
}