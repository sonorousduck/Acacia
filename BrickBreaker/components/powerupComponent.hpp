#pragma once

#include <components/component.hpp>

namespace Powerups
{
	enum PowerupType : std::uint8_t
	{
		LARGER_PADDLE = 1,
		SLOWMO_REFILL = 2,
		ADDITIONAL_BALL = 4
	};
}


namespace components
{
	class Powerup : public PolymorphicComparable<Component, Powerup>
	{
	public:
		Powerup(Powerups::PowerupType powerupType) : powerupType(powerupType)
		{
		}

		Powerups::PowerupType powerupType;

	};
}