#include "ecologyTiledProcessor.hpp"
#include <misc/renderLayers.hpp>


namespace SpaceGuy
{

	void SpaceGuyTiledProcessor::ParseMap(const char* filepath)
	{
		//tson::Tileson t;
		//std::unique_ptr<tson::Map> map = t.parse(fs::path(filepath));

		//if (map->getStatus() == tson::ParseStatus::OK)
		//{
		//	tson::Layer* tileLayer = map->getLayer("Tile Layer 1");
		//	tson::Tileset* tileset = map->getTileset("SpaceGuyTileset");
		//		

		//	for (auto& [pos, tileObject] : tileLayer->getTileObjects())
		//	{
		//		tson::Tileset* tileset = tileObject.getTile()->getTileset();
		//		tson::Rect drawingRect = tileObject.getDrawingRect();
		//		tson::Vector2f position = tileObject.getPosition();

		//		// We have to do a bit of calculations to convert from the drawingRect way of storing it to the correct layer in the spritesheet
		//		int col = drawingRect.x / drawingRect.width;
		//		int row = drawingRect.y / drawingRect.height;

		//		int index = tileset->getColumns() * row + col;

		//		entities::EntityPtr renderedTile = std::make_shared<entities::Entity>();

		//		auto transform = std::make_unique<components::Transform>(glm::vec2(position.x, position.y), 0.0f, glm::vec2(drawingRect.width, drawingRect.height));
		//		auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("spritesheet"), Ebony::ResourceManager::GetTexture("SpaceGuy_spritesheet"), Ebony::Colors::White, Ebony::RenderLayer::BACKGROUND, false, 0.0f, index, true);

		//		renderedTile->addComponent(std::move(transform));
		//		renderedTile->addComponent(std::move(sprite));

		//		Ebony::SystemManager::AddEntity(renderedTile);
		//	}


		//	tson::Layer* enemyLayer = map->getLayer("Enemies");


		//	for (auto& enemy : enemyLayer->getObjects())
		//	{
		//		tson::Vector2i position = enemy.getPosition();
		//		glm::vec2 glmPosition = glm::vec2(position.x, position.y);

		//		translationFunction(glmPosition, enemy.getGid());
		//	}

	}

	void SpaceGuyTiledProcessor::CreateTranslationFunction()
	{
		/*translationFunction = [=](glm::vec2 position, int entityId) 
			{
				switch (entityId)
				{
				case 577:
					Ebony::SystemManager::AddEntity(SpaceGuy::Bat::Create(position, glm::vec2(1.0f), player));
					break;
				case 578:
					Ebony::SystemManager::AddEntity(SpaceGuy::SuicideBird::Create(position, glm::vec2(1.0f), player));
					break;
				default:
					std::cout << "Unknown id: " << entityId << std::endl;
					break;

				}
			};*/
	}
}
