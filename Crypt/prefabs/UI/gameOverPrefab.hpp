#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/text.hpp>
#include <misc/resourceManager.hpp>
#include <misc/renderLayers.hpp>

namespace Crypt
{
	class GameOverPrefab
	{
	public:
		static entities::EntityPtr Create(int windowWidth)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto text = std::make_unique<components::Text>("THIS IS YOUR", Ebony::Colors::Red, Ebony::Colors::Black, Ebony::ResourceManager::GetFont("evil_empire_36"), Ebony::RenderLayer::UI_RENDER);

			auto centeredXPosition = (static_cast<float>(windowWidth) - text->stringSize.x) / 2.0f;

			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(centeredXPosition - 10.0f, 100.0f), 0.0f, glm::vec2(1.0f, 1.0f)));
			entity->addComponent(std::move(text));


			return entity;
		}
	};
}