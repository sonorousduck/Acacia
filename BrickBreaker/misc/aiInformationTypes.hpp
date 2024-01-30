#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace BrickBreaker
{
	enum AiInformationTypes : std::uint64_t
	{
		PADDLE_INFORMATION = BIT(0),
		BALL_INFORMATION = BIT(1),
		BRICK_INFORMATION = BIT(2),
		POWERUP_INFORMATION = BIT(3)
	};
}