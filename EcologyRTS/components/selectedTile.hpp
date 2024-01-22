#pragma once

#include <components/component.hpp>
#include "../misc/tileType.hpp"

namespace components
{
	class SelectedTile : public PolymorphicComparable<Component, SelectedTile>
	{
	public:
		SelectedTile(EcologyRTS::TileType tileType) : tileType(tileType)
		{}

		EcologyRTS::TileType tileType;
		bool isActive = false;


		
	};
}
