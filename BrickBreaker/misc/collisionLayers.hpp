#pragma once
#include <cstdint>


namespace BrickBreaker
{
	enum CollisionLayers : std::uint16_t
	{
		WALL = 1,
		TOP_WALL = 2,
		BRICK = 4,
		PADDLE = 8,
		ALL = 15,
		TEST = 16
	};
}