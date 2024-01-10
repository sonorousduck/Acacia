#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/destructionComponent.hpp>
#include "../scripts/healthBarUpdateScript.hpp"
#include <misc/renderLayers.hpp>

namespace Crypt
{
	class HealthBar
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, entities::EntityPtr parent)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::Red, Ebony::RenderLayer::FOREGROUND);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			

			entity->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					entity->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));


			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::HealthBarUpdateScript>(parent);



			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(script));
			

			return entity;
		}

	};
}