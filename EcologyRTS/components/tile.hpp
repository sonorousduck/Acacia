#pragma once

#include <components/component.hpp>
#include "../misc/tileType.hpp"
#include <misc/resourceManager.hpp>


namespace components
{
	class Tile : public PolymorphicComparable<Component, Tile>
	{
	public:
		Tile(EcologyRTS::TileType tileType, const char* textureName) : tileType(tileType)
		{
			texture = Ebony::ResourceManager::GetTexture(textureName);
		
		}

		Tile(EcologyRTS::TileType tileType, std::shared_ptr<Texture2D> texture) : tileType(tileType), texture(texture)
		{
		}

		EcologyRTS::TileType tileType;
		std::shared_ptr<Texture2D> texture;


		
	};
}
