#pragma once
#include <entity.hpp>
#include <glm/glm.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>
#include <components/transform.hpp>
#include <components/mouseInputComponent.hpp>
#include "../scripts/crosshairPositionScript.hpp"


namespace Crypt
{
	class Crosshair
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, entities::EntityPtr player)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("aim"), Ebony::Colors::White, 0.1);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(25.0f, 25.0f));


			auto mouseComponent = std::make_unique<components::MouseInput>();

			mouseComponent->bindings.insert({ SDL_BUTTON_LEFT, "mousePress" });
			
			
			mouseComponent->onPressActions.insert({ "mousePress", [=](entities::EntityPtr, Ebony::MousePress& mousePress) {std::cout << "Button pressed at " << mousePress.mouseClick.x << ", " << mousePress.mouseClick.y << std::endl; } });
			mouseComponent->onReleaseActions.insert({ "mousePress",[=](entities::EntityPtr, Ebony::MousePress&) {std::cout << "Mouse Button released!" << std::endl; } });

			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::CrosshairScript>(player);

			entity->addComponent(std::move(mouseComponent));
			entity->addComponent(std::move(script));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));

			return entity;
		}

	};
}