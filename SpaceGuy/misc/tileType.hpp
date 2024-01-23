#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace SpaceGuy
{
	enum TileType : std::uint32_t
	{
		FOREST = BIT(0),
        PLAINS = BIT(1),
        MOUNTAINS = BIT(2),
		OCEAN = BIT(3),
		LAKE = BIT(4),
		TUNDRA = BIT(5),
		NONE = BIT(6)


	};
}