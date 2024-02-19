#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace SpaceGuy
{
	enum AiInformationTypes : std::uint64_t
	{
		PLAYER_INFORMATION = BIT(0),
		ENEMY_INFORMATION = BIT(2),
		KEY_INFORMATION = BIT(3),
		ENEMY_BULLET_INFORMATION = BIT(4)
	};
}