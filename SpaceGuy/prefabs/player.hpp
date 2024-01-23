#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>

#include <components/keyboardComponent.hpp>
#include <components/controllerComponent.hpp>

#include "../misc/collisionLayers.hpp"


#include <SDL.h>
#include <graphics2d.hpp>
#include <singletons/time.hpp>

namespace SpaceGuy
{
	class Player
	{
	public:
		static entities::EntityPtr Create()
		{
			auto playerBaseSpeed = 200.0f;
			auto playerBoostSpeed = 1.5f;



			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto texture = Ebony::ResourceManager::GetTexture("player");
			auto scale = texture->getDimensions();


			auto transform = std::make_unique<components::Transform>(glm::vec2(240.0f, 155.0f), 0.0f, scale);
			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto rigidbody = std::make_unique<components::RigidBody>();

			components::Subcollider aabbcollider = components::Subcollider(scale / 2.0f, scale, true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, SpaceGuy::CollisionLayers::PLAYER, SpaceGuy::CollisionLayers::ENEMY | SpaceGuy::CollisionLayers::WALL | SpaceGuy::CollisionLayers::ENEMY_BULLET, false);

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
					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(0.0f, -playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat()));

			}
			});

			keyboardComponent->onHeldActions.insert({ "moveDown", [=]()
			{
					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2{ 0.0f, playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat() });
			}
			});

			keyboardComponent->onHeldActions.insert({ "moveLeft", [=]()
				{
					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(-playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat(), 0.0f));

				}
			});

			keyboardComponent->onHeldActions.insert({ "moveRight", [=]()
				{
					entity->getComponent<components::RigidBody>()->addScriptedMovement(glm::vec2(playerBaseSpeed * Ebony::Time::GetDeltaTimeFloat(), 0.0f));
				}
			});

		

			entity->addComponent(std::move(keyboardComponent));
			entity->addComponent(std::move(controllerComponent));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(rigidbody));
			entity->addComponent(std::move(collider));


			return entity;
		}
	};
}