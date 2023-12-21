#pragma once

#include <glm/glm.hpp>
#include <components/component.hpp>

namespace components
{
	class Shooting : public PolymorphicComparable<Component, Shooting>
	{
	public:
		Shooting()
		{}

		Shooting(float maxShootingSpeed) : maxShootingSpeed(maxShootingSpeed)
		{}


		float maxShootingSpeed = 1.0f;
		float currentCooldown = 0.0f;
	};
}
