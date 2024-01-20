#pragma once
#include <cstdint>

#define BIT(x) (1 << x)

namespace EcologyRTS
{
	enum CollisionLayers : std::uint16_t
	{
		GROUND = BIT(0),
		UI = BIT(1),
		ANIMAL = BIT(2)


	};
}