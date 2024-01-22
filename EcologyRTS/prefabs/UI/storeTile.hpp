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
#include "../../misc/tileType.hpp"
#include "../../components/tile.hpp"
#include "../../misc/collisionLayers.hpp"
#include "../../components/selectedTile.hpp"

namespace EcologyRTS
{
	class StoreTile
	{
	public:
		static entities::EntityPtr Create(glm::vec2 position, const char* storeFilename, const char* inGameFilename, TileType tileType)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			std::shared_ptr<Texture2D> texture = Ebony::ResourceManager::GetTexture(storeFilename);
			auto scale = texture->getDimensions();

			components::Subcollider subcollider = components::Subcollider(scale / 2.0f, scale, true, true);

			// Create the onCollisionStart
			subcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{

					components::ActiveUICollision* activeUICollision;
					if (other->tryGetComponent<components::ActiveUICollision>(activeUICollision))
					{
						entity->getComponent<components::Sprite>()->spriteColor = Ebony::Color::Color(200, 217, 233, 128);

						activeUICollision->collidingWith = entity;
						//other->getComponent<components::SelectedTile>()->tileType = entity->getComponent<components::Tile>()->tileType;
					}
				};
			subcollider.onCollisionEnd = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					entity->getComponent<components::Sprite>()->spriteColor = Ebony::Colors::White;

					components::ActiveUICollision* activeUICollision;
					if (other->tryGetComponent(activeUICollision) && activeUICollision->collidingWith == entity)
					{
						other->getComponent<components::ActiveUICollision>()->collidingWith = nullptr;
						//other->getComponent<components::SelectedTile>()->tileType = TileType::NONE;

					}
				};



			// Base the button text based on the overall size of the button, using the original positioning of the button as a guide.
			entity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), texture, Ebony::Colors::White, Ebony::RenderLayer::UI_RENDER, true));
			entity->addComponent(std::make_unique<components::Transform>(position, 0.0f, scale));
			entity->addComponent(std::make_unique<components::Tile>(tileType, inGameFilename));
			entity->addComponent(std::make_unique<components::Collider>(subcollider, CollisionLayers::UI, CollisionLayers::UI, true));
			entity->addComponent(std::make_unique<components::RigidBody>());


			return entity;
		}
	};
}