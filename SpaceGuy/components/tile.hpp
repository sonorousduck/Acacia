#pragma once

#include <components/component.hpp>
#include "../misc/tileType.hpp"
#include <misc/resourceManager.hpp>


namespace components
{
	class Tile : public PolymorphicComparable<Component, Tile>
	{
	public:
		Tile(SpaceGuy::TileType tileType, const char* textureName) : tileType(tileType)
		{
			texture = Ebony::ResourceManager::GetTexture(textureName);
		
		}

		Tile(SpaceGuy::TileType tileType, std::shared_ptr<Texture2D> texture) : tileType(tileType), texture(texture)
		{
		}

		SpaceGuy::TileType tileType;
		std::shared_ptr<Texture2D> texture;


		
	};
}
