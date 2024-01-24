#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/text.hpp>
#include <misc/resourceManager.hpp>
#include <misc/renderLayers.hpp>
#include "../../scripts/playerHealthUpdateScript.hpp"


namespace SpaceGuy
{
	class PlayerHealth
	{
	public:
		static entities::EntityPtr Create(int windowHeight, entities::EntityPtr player)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			auto text = std::make_unique<components::Text>("Health: 10/10", Ebony::Color::Color(255, 255, 255), Ebony::Colors::Black, Ebony::ResourceManager::GetFont("evil_empire_12"), Ebony::RenderLayer::UI_RENDER);
			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::PlayerHealthScript>(player);

			//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("player"), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND, true);

			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(16.5f, windowHeight - 16.0f), 0.0f, glm::vec2(1.0f, 1.0f)));
			entity->addComponent(std::move(text));
			entity->addComponent(std::move(script));
			//entity->addComponent(std::move(sprite));
			return entity;
		}
	};
}