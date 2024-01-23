#pragma once

#include <components/component.hpp>
#include "../misc/tileType.hpp"

namespace components
{
	class SelectedTile : public PolymorphicComparable<Component, SelectedTile>
	{
	public:
		SelectedTile(SpaceGuy::TileType tileType) : tileType(tileType)
		{}

		SpaceGuy::TileType tileType;
		bool isActive = false;


		
	};
}
