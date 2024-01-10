#pragma once

#include <cstdint>

#include "../Core.hpp"

namespace Ebony
{
	enum RenderLayer : std::uint16_t
	{
		BACKGROUND = BIT(0),
		NEAR_BACKGROUND = BIT(1),
		FOREGROUND = BIT(2),
		UI = BIT(3),
		ALWAYS_FRONT = BIT(4),
		DYNAMIC_PLACING = BIT(5)
	};
}