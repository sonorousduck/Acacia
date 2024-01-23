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

#include <graphics2d.hpp>

namespace EcologyRTS
{
	class MountainTile
	{
	public:
		static entities::EntityPtr Create(entities::EntityPtr cursor)
		{
			entities::EntityPtr tile = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture("mountains_tile");
			auto scale = texture->getDimensions();

			components::Subcollider subcollider = components::Subcollider(scale / 2.0f, scale, true, true);	
			auto position = cursor->getComponent<components::Transform>()->position - (scale / 2.0f) + glm::vec2(Ebony::Graphics2d::mainCamera->Position);

			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.
			tile->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND, false));
			tile->addComponent(std::make_unique<components::Transform>(position, 0.0f, scale));
			tile->addComponent(std::make_unique<components::Tile>(TileType::MOUNTAINS, "mountains_tile"));
			tile->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::GROUND, CollisionLayers::GROUND, true));
			tile->addComponent(std::make_unique<components::RigidBody>());

			return tile;
		}
	};
}