#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace Crypt
{
	enum CollisionLayers : std::uint16_t
	{
		GROUND = BIT(0),
		UI = BIT(1),
		PLAYER = BIT(2),
		ENEMY = BIT(3),
		PLAYER_BULLET = BIT(4),
		ENEMY_BULLET = BIT(5),
		ENDING_WALL = BIT(6)


	};
}