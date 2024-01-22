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
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture("mountains_tile");
			auto scale = texture->getDimensions();

			components::Subcollider subcollider = components::Subcollider(scale / 2.0f, scale, true, true);	


			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND, false));
			entity->addComponent(std::make_unique<components::Transform>(cursor->getComponent<components::Transform>()->position - (scale / 2.0f) + glm::vec2(Ebony::Graphics2d::mainCamera->Position), 0.0f, scale));
			entity->addComponent(std::make_unique<components::Tile>(TileType::MOUNTAINS, "mountains_tile"));
			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::GROUND, CollisionLayers::GROUND, true));
			entity->addComponent(std::make_unique<components::RigidBody>());

			return entity;
		}
	};
}