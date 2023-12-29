#include "cryptTiledProcessor.hpp"


namespace Crypt
{

	void CryptTiledProcessor::ParseMap(const char* filepath, std::function<void(entities::EntityPtr)> AddEntity)
	{
		tson::Tileson t;
		std::unique_ptr<tson::Map> map = t.parse(fs::path(filepath));

		if (map->getStatus() == tson::ParseStatus::OK)
		{
			tson::Layer* tileLayer = map->getLayer("Tile Layer 1");
			tson::Tileset* tileset = map->getTileset("Test_SpriteSheet");
				

			for (auto& [pos, tileObject] : tileLayer->getTileObjects())
			{
				tson::Tileset* tileset = tileObject.getTile()->getTileset();
				tson::Rect drawingRect = tileObject.getDrawingRect();
				tson::Vector2f position = tileObject.getPosition();

				// We have to do a bit of calculations to convert from the drawingRect way of storing it to the correct layer in the spritesheet


				entities::EntityPtr renderedTile = std::make_shared<entities::Entity>();

				auto test = tileset->getImage().u8string();

				auto transform = std::make_unique<components::Transform>(glm::vec2(position.x, position.y), 0.0f, glm::vec2(drawingRect.width, drawingRect.height));
				auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("spritesheet"), Ebony::ResourceManager::GetTexture("test_spritesheet"), Ebony::Colors::White, 0.05f, false, 0, true);

				renderedTile->addComponent(std::move(transform));
				renderedTile->addComponent(std::move(sprite));

				AddEntity(renderedTile);
			}
		}
	}

	void CryptTiledProcessor::CreateTranslationFunction()
	{
		translationFunction = [=](int entityId) 
			{
				switch (entityId)
				{
				case 3221225509:
					std::cout << "Top Wall" << std::endl;
					break;
				case 50:
					std::cout << "Background wall" << std::endl;
					break;
				case 37:
					std::cout << "Bottom wall" << std::endl;
					break;
				default:
					std::cout << "Unknown id: " << entityId << std::endl;
					break;

				}
			};
	}
}
