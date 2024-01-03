#pragma once
#include <entity.hpp>
#include <glm/glm.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>
#include <components/transform.hpp>
#include <components/mouseInputComponent.hpp>
#include "../scripts/crosshairPositionScript.hpp"
#include "../components/aimComponent.hpp"
#include "bulletPrefab.hpp"
#include "../components/shootingComponent.hpp"

namespace Crypt
{
	class Crosshair
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, entities::EntityPtr player,  std::function<void(entities::EntityPtr)> AddEntity)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			float speed = 10.0f;

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("aim"), Ebony::Colors::White, 0.1f);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(25.0f, 25.0f));


			auto mouseComponent = std::make_unique<components::MouseInput>();

			mouseComponent->bindings.insert({ SDL_BUTTON_LEFT, "shoot" });
			

			mouseComponent->onPressActions.insert({ "shoot", [=](Ebony::MousePress& mousePress)
				{
					auto shootingComponent = entity->getComponent<components::Shooting>();

					if (shootingComponent->currentCooldown >= shootingComponent->maxShootingSpeed)
					{
						auto crosshair = entity->getComponent<components::Crosshair>();
						AddEntity(BulletPrefab::Create(player->getComponent<components::Transform>()->position, glm::vec2(3.0f, 3.0f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "fire_bullet"));

						shootingComponent->currentCooldown = 0.0f;
					}

				} });



			auto controllerComponent = std::make_unique<components::ControllerInput>(0);

			
			controllerComponent->joystickBindings.insert({ SDL_CONTROLLER_AXIS_TRIGGERRIGHT, "shoot" });

			controllerComponent->joystickActions.insert({ "shoot", [=](float value)
				{
					auto shootingComponent = entity->getComponent<components::Shooting>();

					if (shootingComponent->currentCooldown >= shootingComponent->maxShootingSpeed)
					{
						auto crosshair = entity->getComponent<components::Crosshair>();
						AddEntity(BulletPrefab::Create(crosshair->aimLocation, glm::vec2(50.0f, 50.0f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "fire_bullet"));

						shootingComponent->currentCooldown = 0.0f;
						Ebony::InputManager::Vibrate(0, 0, 1.0f, 100, true);
					}
				} });


			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::CrosshairScript>(player);

			entity->addComponent(std::move(mouseComponent));
			entity->addComponent(std::move(script));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(controllerComponent));
			entity->addComponent(std::make_unique<components::Crosshair>());
			entity->addComponent(std::make_unique<components::Shooting>());

			return entity;
		}

	};
}