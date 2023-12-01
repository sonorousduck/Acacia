#pragma once
#include <cstdint>


namespace BrickBreaker
{
	// This will be implemented by other games mostly, just not DEFAULT of 0. This is to let the 
	// screen declare the ScreenEnum type (Potentially circle back and just make this a std::uint16_t instead of the ScreenEnum)
	enum ScreenEnum : std::uint16_t
	{
		GAME = 1,
		MAIN_MENU = 2,
		PAUSE = 4,
		GAME_OVER = 8,
		CONTROLS = 16,
		HIGH_SCORE = 32,
		OPTIONS = 64,
		QUIT = 128
	};
}