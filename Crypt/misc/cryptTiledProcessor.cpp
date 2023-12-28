#include <misc/tiledProcessor.hpp>


namespace Crypt
{
	class CryptTiledProcessor : Ebony::TiledProcessor
	{
	public:
		void ParseMap(const char* filepath) override
		{
			std::cout << "Test" << std::endl;
			//tson::Tileson t;
			std::cout << "Probably didn't work!" << std::endl;

			/*std::unique_ptr<tson::Map> map = t.parse(fs::path(filepath));

			if (map->getStatus() == tson::ParseStatus::OK)
			{
				tson::Layer* tileLayer = map->getLayer("Walls");
				tson::Tileset* tileset = map->getTileset("GenericBackground");
				

				for (auto& [pos, tileObject] : tileLayer->getTileObjects())
				{
					tson::Tileset* tileset = tileObject.getTile()->getTileset();
					tson::Rect drawingRect = tileObject.getDrawingRect();
					tson::Vector2f position = tileObject.getPosition();
				}


			}*/
		}

		void CreateTranslationFunction() override
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
	};
}
