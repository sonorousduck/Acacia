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
#include <components/aiInputComponent.hpp>

#include <misc/renderLayers.hpp>
#include "../singletons/CryptPythonManager.hpp"

namespace Crypt
{
	class Crosshair
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, entities::EntityPtr player)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			float speed = 10.0f;

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("aim"), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(25.0f, 25.0f));


			std::unique_ptr<components::AiInput> aiInputComponent = std::make_unique<components::AiInput>();

			aiInputComponent->actions.insert({ "shoot", [=]()
				{
					auto shootingComponent = entity->getComponent<components::Shooting>();

					if (shootingComponent->currentCooldown >= shootingComponent->maxShootingSpeed)
					{
						auto crosshair = entity->getComponent<components::Crosshair>();
						glm::vec2 fireFromPosition = player->getComponent<components::Transform>()->position;

						if (crosshair->bulletType & components::BULLET_TYPE::FIRE)
						{
							Ebony::SystemManager::AddEntity(BulletPrefab::Create(fireFromPosition, glm::vec2(2.5f, 2.5f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "fire_bullet"));
						}
						else if (crosshair->bulletType & components::BULLET_TYPE::ICE)
						{
							Ebony::SystemManager::AddEntity(BulletPrefab::Create(fireFromPosition, glm::vec2(2.5f, 2.5f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "ice_bullet"));
						}


						shootingComponent->currentCooldown = 0.0f;
					}
				} });

			aiInputComponent->translationFunction = [=]()
				{
					// TODO: Update this so it can figure out its instance ID. But for now, just assume it is the first one
					auto& actionList = Crypt::CryptPythonManager::actions[0];

					if (actionList.size() == 0)
					{
						return;
					}

					auto& action = actionList[0];

					if (action.box[3] == 1.0f)
					{
						entity->getComponent<components::AiInput>()->actions["shoot"]();
					}
				};


			auto mouseComponent = std::make_unique<components::MouseInput>();

			mouseComponent->bindings.insert({ SDL_BUTTON_LEFT, "shoot" });
			

			mouseComponent->onPressActions.insert({ "shoot", [=](Ebony::MousePress& mousePress)
				{
					auto shootingComponent = entity->getComponent<components::Shooting>();

					if (shootingComponent->currentCooldown >= shootingComponent->maxShootingSpeed)
					{
						auto crosshair = entity->getComponent<components::Crosshair>();
						glm::vec2 fireFromPosition = player->getComponent<components::Transform>()->position;

						if (crosshair->bulletType & components::BULLET_TYPE::FIRE)
						{
							Ebony::SystemManager::AddEntity(BulletPrefab::Create(fireFromPosition, glm::vec2(2.5f, 2.5f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "fire_bullet"));
						}
						else if (crosshair->bulletType & components::BULLET_TYPE::ICE)
						{
							Ebony::SystemManager::AddEntity(BulletPrefab::Create(fireFromPosition, glm::vec2(2.5f, 2.5f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "ice_bullet"));
						}


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
						glm::vec2 fireFromPosition = player->getComponent<components::Transform>()->position;

						if (crosshair->bulletType & components::BULLET_TYPE::FIRE)
						{
							Ebony::SystemManager::AddEntity(BulletPrefab::Create(fireFromPosition, glm::vec2(2.5f, 2.5f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "fire_bullet"));
						}
						else if (crosshair->bulletType & components::BULLET_TYPE::ICE)
						{
							Ebony::SystemManager::AddEntity(BulletPrefab::Create(fireFromPosition, glm::vec2(2.5f, 2.5f), crosshair->aimDirection, speed, crosshair->bulletType, 1, "ice_bullet"));
						}


						shootingComponent->currentCooldown = 0.0f;
					}
				} });


			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::CrosshairScript>(player);

			entity->addComponent(std::move(mouseComponent));
			entity->addComponent(std::move(script));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(controllerComponent));
			entity->addComponent(std::move(aiInputComponent));
			entity->addComponent(std::make_unique<components::Crosshair>());
			entity->addComponent(std::make_unique<components::Shooting>());

			return entity;
		}

	};
}