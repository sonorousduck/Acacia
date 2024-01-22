#pragma once
#include <memory>
#include <glm/glm.hpp>

#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <misc/renderLayers.hpp>
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include <components/activeUICollisionComponent.hpp>
#include "../misc/tileType.hpp"
#include "../components/tile.hpp"
#include "../misc/collisionLayers.hpp"
#include "../components/selectedTile.hpp"
#include "../scripts/followCursorScript.hpp"

namespace EcologyRTS
{
	class CursorFollowingTile
	{
	public:
		static entities::EntityPtr Create(entities::EntityPtr cursor, TileType tileType)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			std::shared_ptr<Texture2D> texture;
			switch (tileType)
			{
			case (TileType::MOUNTAINS):
			{
				texture = Ebony::ResourceManager::GetTexture("mountains_tile");
				break;

			}
			case (TileType::LAKE):
			{
				//texture = Ebony::ResourceManager::GetTexture(filename);
				break;

			}
			case (TileType::FOREST):
			{
				texture = Ebony::ResourceManager::GetTexture("forest_tile");
				break;

			}
			case (TileType::OCEAN):
			{
				//texture = Ebony::ResourceManager::GetTexture(filename);
				break;

			}
			case (TileType::PLAINS):
			{
				texture = Ebony::ResourceManager::GetTexture("plains_tile");
				break;

			}
			case (TileType::TUNDRA):
			{
				//texture = Ebony::ResourceManager::GetTexture(filename);
				break;

			}
			default:
			{
				break;
			}
		}


			auto scale = texture->getDimensions();

			components::Subcollider subcollider = components::Subcollider(scale / 2.0f, scale, true, true);	

			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::FollowCursorScript>(cursor);


			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::Red, Ebony::RenderLayer::UI_RENDER, true));
			entity->addComponent(std::make_unique<components::Transform>(cursor->getComponent<components::Transform>()->position, 0.0f, scale));
			entity->addComponent(std::make_unique<components::Tile>(tileType, texture));
			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::UI, CollisionLayers::UI, true));
			entity->addComponent(std::make_unique<components::RigidBody>());
			entity->addComponent(std::move(script));

			entity->Disable();

			return entity;
		}
	};
}