#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>

namespace Crypt
{
	class PlayerSpellSelection
	{
	public:
		static entities::EntityPtr Create(int windowHeight, std::function<void(entities::EntityPtr entity)> AddEntity)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("panel"), Ebony::Colors::White, 0.20f, true);
			//std::unique_ptr<components::CppScript> script = std::make_unique<scripts::PlayerHealthUpdateScript>(player);


			
			entity->addComponent(std::move(std::make_unique<components::Transform>(glm::vec2{ 0.0f, static_cast<float>(windowHeight) - sprite->texture->Height - 10.0f}, 0.0f, sprite->GetDimensions())));
			entity->addComponent(std::move(sprite));

			//auto textComponent = std::make_unique<components::Text>("10/10", Ebony::Colors::Black, Ebony::Colors::White, Ebony::ResourceManager::GetFont("evil_empire"), 0.15f, true, true, glm::vec2(1.0f, 1.0f));
			


			return entity;
		}
	};
}