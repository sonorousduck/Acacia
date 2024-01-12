#pragma once

#include <entity.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <components/text.hpp>
#include <components/transform.hpp>
#include "../components/scoreComponent.hpp"

namespace BrickBreaker
{
	class ScorePrefab
	{
	public:
		static entities::EntityPtr Create(glm::vec2 position, std::shared_ptr<Ebony::SpriteFont> spriteFont)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			auto textComponent = std::make_unique<components::Text>("Score", Ebony::Colors::Black, Ebony::Colors::White, spriteFont, Ebony::RenderLayer::UI_RENDER);
			entity->addComponent(std::move(textComponent));
			entity->addComponent(std::move(std::make_unique<components::Transform>(position, 0.0f, glm::vec2{ 1.0f, 1.0f })));
			entity->addComponent(std::make_unique<components::Score>());

			return entity;
		}
	};
}