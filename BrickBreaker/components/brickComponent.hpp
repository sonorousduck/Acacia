#pragma once

#include <components/component.hpp>


namespace components
{
	class Brick : public PolymorphicComparable<Component, Brick>
	{
	public:
		Brick(int strength, std::uint32_t pointValue) :
			strength(strength), 
			pointValue(pointValue)
		{}

		bool hitAlready = false;
		int strength;
		std::uint32_t pointValue;
		bool destroyed = false;
	};
}