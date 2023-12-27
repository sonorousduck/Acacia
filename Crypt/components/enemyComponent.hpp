#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

#include <Core.hpp>

namespace components
{

	// Potentially, introduce weaknesses to certain power types
	class Enemy : public PolymorphicComparable<Component, Enemy>
	{
	public:
		Enemy(float health) : health(health), maxHealth(health)
		{}


		float health;
		float maxHealth;

	};
}
