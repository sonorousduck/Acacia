#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/text.hpp>
#include <misc/resourceManager.hpp>
#include <misc/renderLayers.hpp>

namespace Crypt
{
	class VictoryPrefab
	{
	public:
		static entities::EntityPtr Create(int windowWidth)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto text = std::make_unique<components::Text>("VICTORY!", Ebony::Colors::Red, Ebony::Colors::Black, Ebony::ResourceManager::GetFont("default"), Ebony::RenderLayer::UI_RENDER);

			auto centeredXPosition = (static_cast<float>(windowWidth) - text->stringSize.x) / 2.0f;


			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.


			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(centeredXPosition, 100.0f), 0.0f, glm::vec2(1.0f, 1.0f)));
			entity->addComponent(std::move(text));


			return entity;
		}
	};
}