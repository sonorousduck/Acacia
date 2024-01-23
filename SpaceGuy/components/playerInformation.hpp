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
		float score = 0.0f;
		bool hasSpeedBoost = false;



		
	};
}
