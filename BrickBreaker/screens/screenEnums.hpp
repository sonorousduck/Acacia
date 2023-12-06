#pragma once
#include <cstdint>
#include "Core.hpp"

namespace BrickBreaker
{
	// This will be implemented by other games mostly, just not DEFAULT of 0. This is to let the 
	// screen declare the ScreenEnum type (Potentially circle back and just make this a std::uint16_t instead of the ScreenEnum)
	enum ScreenEnum : std::uint64_t
	{
		GAME = BIT(1),
		MAIN_MENU = BIT(2),
		PAUSE = BIT(3),
		GAME_OVER = BIT(4),
		CONTROLS = BIT(5),
		HIGH_SCORE = BIT(6),
		OPTIONS = BIT(7),
		QUIT = BIT(8)
	};
}