#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace SpaceGuy
{
	enum AiInformationTypes : std::uint64_t
	{
		PLAYER_INFORMATION = BIT(0),
		CROSSHAIR_INFORMATION = BIT(1),
		ENEMY_INFORMATION = BIT(2),
		BULLET_INFORMATION = BIT(3)
	};
}