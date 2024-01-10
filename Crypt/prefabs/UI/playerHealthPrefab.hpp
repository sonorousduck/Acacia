#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>
#include <iostream>
#include "../../scripts/playerHealthUpdateScript.hpp"
#include "heartPrefab.hpp"
#include <misc/renderLayers.hpp>

namespace Crypt
{
	class PlayerHealth
	{
	public:
		static entities::EntityPtr Create(entities::EntityPtr player)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("heart_ui_background"), Ebony::Colors::White, Ebony::RenderLayer::UI_RENDER, true);
			auto scale = sprite->GetDimensions();

			entity->addComponent(std::move(std::make_unique<components::Transform>(glm::vec2{ 0.0f, 25.0f }, 0.0f, scale)));
			entity->addComponent(std::move(sprite));


			auto heart1 = Heart::Create(glm::vec2(0.0f, 30.0f));
			auto heart2 = Heart::Create(glm::vec2(16.0f, 30.0f));
			auto heart3 = Heart::Create(glm::vec2(32.0f, 30.0f));

			Ebony::SystemManager::AddEntity(heart1);
			Ebony::SystemManager::AddEntity(heart2);
			Ebony::SystemManager::AddEntity(heart3);



			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::PlayerHealthUpdateScript>(player, heart1, heart2, heart3);

			std::unique_ptr<components::PythonScript> pythonScript = std::make_unique<components::PythonScript>("testPythonScript");


			entity->addComponent(std::move(script));
			entity->addComponent(std::move(pythonScript));



			return entity;
		}
	};
}