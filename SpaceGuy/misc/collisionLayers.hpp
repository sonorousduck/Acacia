#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace SpaceGuy
{
	enum CollisionLayers : std::uint16_t
	{
		ENEMY_BULLET = BIT(0),
		UI = BIT(1),
		WALL = BIT(2),
		ENEMY = BIT(3),
		PLAYER = BIT(4),
		PLAYER_BULLET = BIT(5)


	};
}