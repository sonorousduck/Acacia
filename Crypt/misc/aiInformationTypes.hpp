#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace Crypt
{
	enum AiInformationTypes : std::uint64_t
	{
		PLAYER_INFORMATION = BIT(0),
		ENEMY_INFORMATION = BIT(1)
	};
}