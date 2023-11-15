#pragma once

#include <components/component.hpp>


namespace components
{
	class Brick : public PolymorphicComparable<Component, Brick>
	{
	public:
		Brick(int strength, float pointValue) : 
			strength(strength), 
			pointValue(pointValue)
		{}


		int strength;
		float pointValue;
	};
}