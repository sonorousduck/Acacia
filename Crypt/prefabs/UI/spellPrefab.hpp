#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>
#include <iostream>
#include "../../scripts/playerHealthUpdateScript.hpp"
#include <misc/renderLayers.hpp>

namespace Crypt
{
	class SpellPrefab
	{
	public:
		static entities::EntityPtr Create(entities::EntityPtr spellSelectionUI, glm::vec2 position, glm::vec2 scaling, const char* imageName, bool startingSpell = false)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(imageName), startingSpell ? Ebony::Color(glm::vec4(0.5647f, 0.933f, 0.5647f, 0.6f)) : Ebony::Colors::White, Ebony::RenderLayer::ALWAYS_FRONT, true);

			entity->addComponent(std::make_unique<components::Transform>(position, 0.0f, sprite->GetDimensions() * scaling));
			entity->addComponent(std::move(sprite));
			//entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Color(1.0f, 1.0f, 1.0f, 1.0f), 0.1f, true));


			return entity;
		}
	};
}