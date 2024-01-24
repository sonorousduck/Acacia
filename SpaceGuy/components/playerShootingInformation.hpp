#pragma once

#include <components/component.hpp>

namespace components
{
	class PlayerShootingInformation : public PolymorphicComparable<Component, PlayerShootingInformation>
	{
	public:
		PlayerShootingInformation()
		{}

		std::uint8_t missileCount = 4;
		bool hasShotgun = false;
		bool hasRapidFire = true;
		bool canShoot = true;



		
	};
}
