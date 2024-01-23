#pragma once
#include <components/mouseInputComponent.hpp>
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/collider.hpp>
#include <components/subcolliderClass.hpp>
#include <components/rigidbodyComponent.hpp>
#include "../misc/collisionLayers.hpp"
#include <iostream>
#include <components/activeUICollisionComponent.hpp>
#include <components/buttonComponent.hpp>
#include "../singletons/GameManager.hpp"
#include "../prefabs/cursorFollowingTile.hpp"
#include "../components/selectedTile.hpp"

#include "../scripts/followCursorScript.hpp"
#include <singletons/systemManager.hpp>


#include "mountainTile.hpp"

namespace EcologyRTS
{
	class MainGameCursor
	{
	public:
		static entities::EntityPtr Create(entities::EntityPtr inventory)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("cursor"), Ebony::Colors::White, Ebony::RenderLayer::ALWAYS_FRONT, true);
			auto scale = sprite->GetDimensions();
			entity->addComponent(std::make_unique<components::Transform>(glm::vec2(0.0f, 0.0f), 0.0f, scale));



			components::Subcollider subcollider = components::Subcollider(scale / 2.0f, scale, true, true);
			entity->addComponent(std::make_unique<components::RigidBody>());


			auto mouseInput = std::make_unique<components::MouseInput>();

			mouseInput->onMove = [=]()
				{
					auto mouseInput = entity->getComponent<components::MouseInput>();

					entity->getComponent<components::Transform>()->position = glm::vec2(mouseInput->positionX, mouseInput->positionY);
				};

			mouseInput->bindings.insert({ SDL_BUTTON_LEFT, "pressButton" });


			mouseInput->onPressActions.insert({ "pressButton", [=](Ebony::MousePress& mousePress)
				{
					/*if (inventory->isEnabled())
					{*/
						auto activeCollisionComponent = entity->getComponent<components::ActiveUICollision>();

						if (activeCollisionComponent->collidingWith != nullptr)
						{
							// Selection of the tile based on what was clicked
							auto tileInformation = activeCollisionComponent->collidingWith->getComponent<components::Tile>();

							entities::EntityPtr cursorFollower = entity->GetChild(0);
							cursorFollower->getComponent<components::Sprite>()->texture = tileInformation->texture;
							cursorFollower->getComponent<components::Tile>()->tileType = tileInformation->tileType;
							cursorFollower->Enable();
							

							auto selectedTile = entity->getComponent<components::SelectedTile>();

							selectedTile->tileType = tileInformation->tileType;
							selectedTile->isActive = true;

							return;
						}

						auto selectedTile = entity->getComponent<components::SelectedTile>();
						if (selectedTile->tileType != TileType::NONE && selectedTile->isActive)
						{
							// If valid placement
							Ebony::SystemManager::AddEntity(EcologyRTS::MountainTile::Create(entity));
							std::cout << "Placed" << std::endl;
							selectedTile->isActive = false;


							entity->GetChild(0)->Disable();
							
							return;
						}

						//return;
					//}
				} });



			entity->addComponent(std::move(mouseInput));
			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::UI | CollisionLayers::GROUND, false));
			entity->addComponent(std::make_unique<components::RigidBody>());
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::make_unique<components::ActiveUICollision>());
			// RIGHT NOW, THIS IS DEFAULTING TO SOMETHING SINCE I DON"T HAVE THE ACTUAL SELECTION DONE
			entity->addComponent(std::make_unique<components::SelectedTile>(EcologyRTS::MOUNTAINS));



			return entity;
		}
	};
}