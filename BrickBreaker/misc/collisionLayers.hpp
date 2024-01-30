#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace BrickBreaker
{
	enum CollisionLayers : std::uint16_t
	{
		WALL = BIT(0),
		TOP_WALL = BIT(1),
		BRICK = BIT(2),
		PADDLE = BIT(3),
		POWERUP = BIT(4),
		UI = BIT(5),
		BALL = BIT(6)
	};
}