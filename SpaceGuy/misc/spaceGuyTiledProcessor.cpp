#include "spaceGuyTiledProcessor.hpp"
#include <misc/renderLayers.hpp>
#include <singletons/systemManager.hpp>
#include "../prefabs/smallEnemyPrefab.hpp"
#include "../prefabs/largerEnemyPrefab.hpp"
#include "../prefabs/spawnerPrefab.hpp"
#include "../prefabs/largerEnemyPrefab.hpp"
#include "../prefabs/turretPrefab.hpp"
#include "../prefabs/keyPrefab.hpp"
#include "../prefabs/lockedDoorPrefab.hpp"
#include "../prefabs/UI/playerMissilePrefab.hpp"
#include "../prefabs/speedBoostPrefab.hpp"
#include "../prefabs/potionPrefab.hpp"
#include "../prefabs/missileRefillPrefab.hpp"
#include "../prefabs/shotgunPrefab.hpp"
#include "../prefabs/rapidFirePrefab.hpp"

namespace SpaceGuy
{

	void SpaceGuyTiledProcessor::ParseMap(const char* filepath)
	{
		tson::Tileson t;
		std::unique_ptr<tson::Map> map = t.parse(fs::path(filepath));

		if (map->getStatus() == tson::ParseStatus::OK)
		{
			tson::Layer* tileLayer = map->getLayer("Tile Layer 1");
			tson::Tileset* tileset = map->getTileset("MainSpriteSheet");


			for (auto& [pos, tileObject] : tileLayer->getTileObjects())
			{
				tson::Tileset* tileset = tileObject.getTile()->getTileset();
				tson::Rect drawingRect = tileObject.getDrawingRect();
				tson::Vector2f position = tileObject.getPosition();

				// We have to do a bit of calculations to convert from the drawingRect way of storing it to the correct layer in the spritesheet
				int col = drawingRect.x / drawingRect.width;
				int row = drawingRect.y / drawingRect.height;

				int index = tileset->getColumns() * row + col;

				entities::EntityPtr renderedTile = std::make_shared<entities::Entity>();

				auto transform = std::make_unique<components::Transform>(glm::vec2(position.x, position.y), 0.0f, glm::vec2(drawingRect.width, drawingRect.height));
				auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("spritesheet"), Ebony::ResourceManager::GetTexture("main_spritesheet"), Ebony::Colors::White, Ebony::RenderLayer::BACKGROUND, false, 0.0f, index, true);

				renderedTile->addComponent(std::move(transform));
				renderedTile->addComponent(std::move(sprite));

				Ebony::SystemManager::AddEntity(renderedTile);
			}


			tson::Layer* enemyLayer = map->getLayer("Enemies");


			for (auto& enemy : enemyLayer->getObjects())
			{
				tson::Vector2i position = enemy.getPosition();
				glm::vec2 glmPosition = glm::vec2(position.x, position.y);

				translationFunction(glmPosition, enemy.getGid());
			}

			tson::Layer* colliderLayer = map->getLayer("Colliders");


			for (auto& collider : colliderLayer->getObjects())
			{
				tson::Vector2i position = collider.getPosition();
				tson::Vector2i drawingRect = collider.getSize();
				glm::vec2 glmPosition = glm::vec2(position.x, position.y);
				glm::vec2 scale = glm::vec2(drawingRect.x, drawingRect.y);
				
				entities::EntityPtr wall = std::make_shared<entities::Entity>();
				wall->addComponent(std::make_unique<components::Transform>(glmPosition, 0.0f, scale));
				components::Subcollider aabbcollider = components::Subcollider(scale / 2.0f, scale, true, true);

				aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
					{
						other->getComponent<components::Transform>()->position = other->getComponent<components::Transform>()->previousPosition;
					};

				aabbcollider.onCollision = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
					{
						other->getComponent<components::Transform>()->position = other->getComponent<components::Transform>()->previousPosition;
					};


				auto collider = std::make_unique<components::Collider>(aabbcollider, SpaceGuy::CollisionLayers::WALL, SpaceGuy::CollisionLayers::PLAYER | SpaceGuy::CollisionLayers::ENEMY, true);
				//auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::Red, Ebony::ALWAYS_FRONT);

				wall->addComponent(std::move(collider));
				wall->addComponent(std::make_unique<components::RigidBody>());
				//wall->addComponent(std::move(sprite));

				Ebony::SystemManager::AddEntity(wall);
			}

		}
	}

	void SpaceGuyTiledProcessor::CreateTranslationFunction()
	{
		translationFunction = [=](glm::vec2 position, int entityId) 
			{
				switch (entityId)
				{
				case 26:
					Ebony::SystemManager::AddEntity(SpaceGuy::MissileRefill::Create(position));
					break;
				case 28:
					Ebony::SystemManager::AddEntity(SpaceGuy::SpeedBoost::Create(position));
					break;
				case 29:
					Ebony::SystemManager::AddEntity(SpaceGuy::RapidFire::Create(position));
					break;
				case 30:
					Ebony::SystemManager::AddEntity(SpaceGuy::Shotgun::Create(position));
					break;
				case 33:
					Ebony::SystemManager::AddEntity(SpaceGuy::SmallEnemy::Create(position, glm::vec2(1.0f), player));
					break;
				case 35:
					Ebony::SystemManager::AddEntity(SpaceGuy::LargerEnemy::Create(position, glm::vec2(1.0f), player));
					break;
				case 37:
					Ebony::SystemManager::AddEntity(SpaceGuy::Spawner::Create(position, glm::vec2(1.0f), player));
					break;
				case 38:
					Ebony::SystemManager::AddEntity(SpaceGuy::Turret::Create(position, glm::vec2(1.0f), player));
					break;
				case 45:
					Ebony::SystemManager::AddEntity(SpaceGuy::LockedDoor::Create(position));
					break;
				case 47:
					Ebony::SystemManager::AddEntity(SpaceGuy::Potion::Create(position));
					break;
				case 48:
					Ebony::SystemManager::AddEntity(SpaceGuy::Key::Create(position));
					break;
				default:
					std::cout << "Unknown id: " << entityId << std::endl;
					break;

				}
			};
	}
}
