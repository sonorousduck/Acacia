#pragma once

#include <components/component.hpp>

namespace components
{
	class PlayerInformation : public PolymorphicComparable<Component, PlayerInformation>
	{
	public:
		PlayerInformation()
		{}


		float health = 10.0f;
		float maxHealth = 10.0f;
		std::uint8_t lives = 3;
		float score = 0.0f;
		bool hasSpeedBoost = false;
		float speedBoostTime = 0.0f;
		bool hasKey = false;


		
	};
}
