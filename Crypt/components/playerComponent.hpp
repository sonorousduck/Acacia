#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

namespace components
{
	class Player : public PolymorphicComparable<Component, Player>
	{
	public:
		Player()
		{}


		bool isOnGround = false;
	};
}
