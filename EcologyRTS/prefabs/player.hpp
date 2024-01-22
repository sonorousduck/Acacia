#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>

#include <components/keyboardComponent.hpp>
#include <components/controllerComponent.hpp>

#include <SDL.h>
#include <graphics2d.hpp>
#include <singletons/time.hpp>

namespace EcologyRTS
{
	class Player
	{
	public:
		static entities::EntityPtr Create(entities::EntityPtr inventoryScreen)
		{
			auto cameraSpeed = 1000.0f;

			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto keyboardComponent = std::make_unique<components::KeyboardInput>();
			auto controllerComponent = std::make_unique<components::ControllerInput>(0);

			keyboardComponent->bindings.insert({ SDLK_w, "moveUp" });
			keyboardComponent->bindings.insert({ SDLK_a, "moveLeft" });
			keyboardComponent->bindings.insert({ SDLK_d, "moveRight" });
			keyboardComponent->bindings.insert({ SDLK_s, "moveDown" });
			keyboardComponent->bindings.insert({ SDLK_i, "openInventory" });
			keyboardComponent->bindings.insert({ SDLK_ESCAPE, "pauseOrCloseInventory" });

			keyboardComponent->onHeldActions.insert({ "moveUp", [=]()
			{
					auto cameraPosition = Ebony::Graphics2d::mainCamera->Position;
					cameraPosition.y -= cameraSpeed * Ebony::Time::GetDeltaTimeFloat();
					Ebony::Graphics2d::mainCamera->SetCameraPosition(cameraPosition);
			}
			});

			keyboardComponent->onHeldActions.insert({ "moveDown", [=]()
			{
					auto cameraPosition = Ebony::Graphics2d::mainCamera->Position;
					cameraPosition.y += cameraSpeed * Ebony::Time::GetDeltaTimeFloat();
					Ebony::Graphics2d::mainCamera->SetCameraPosition(cameraPosition);
			}
			});

			keyboardComponent->onHeldActions.insert({ "moveLeft", [=]()
				{
						auto cameraPosition = Ebony::Graphics2d::mainCamera->Position;
						cameraPosition.x -= cameraSpeed * Ebony::Time::GetDeltaTimeFloat();
						Ebony::Graphics2d::mainCamera->SetCameraPosition(cameraPosition);
				}
			});

			keyboardComponent->onHeldActions.insert({ "moveRight", [=]()
					{
							auto cameraPosition = Ebony::Graphics2d::mainCamera->Position;
							cameraPosition.x += cameraSpeed * Ebony::Time::GetDeltaTimeFloat();
							Ebony::Graphics2d::mainCamera->SetCameraPosition(cameraPosition);
					}
			});

			keyboardComponent->onPressActions.insert({ "openInventory", [=]()
				{
					inventoryScreen->SetEnabledStatus(!inventoryScreen->isEnabled());
				} 
			});

			keyboardComponent->onPressActions.insert({ "pauseOrCloseInventory", [=]()
				{
					if (inventoryScreen->isEnabled())
					{
						//inventoryScreen->Disable();
					}
					else
					{
						// Open Pause menu
					}
					
				}
			});



			entity->addComponent(std::move(keyboardComponent));
			entity->addComponent(std::move(controllerComponent));

			return entity;
		}
	};
}