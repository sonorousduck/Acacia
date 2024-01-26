#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/text.hpp>
#include <misc/resourceManager.hpp>
#include <misc/renderLayers.hpp>
#include "../../scripts/playerOverallLivesText.hpp"


namespace SpaceGuy
{
	class PlayerLivesTextPrefab
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startPosition, entities::EntityPtr player)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			auto text = std::make_unique<components::Text>("x 0", Ebony::Colors::Yellow, Ebony::Colors::Black, Ebony::ResourceManager::GetFont("evil_empire_12"), Ebony::RenderLayer::UI_RENDER);
			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::PlayerOverallLivesScript>(player);

			entity->addComponent(std::make_unique<components::Transform>(startPosition + glm::vec2(12.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f)));
			entity->addComponent(std::move(text));
			entity->addComponent(std::move(script));
			return entity;
		}
	};
}