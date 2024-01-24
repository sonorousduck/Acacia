#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

#include <Core.hpp>

namespace components
{

	// Potentially, introduce weaknesses to certain power types
	class EnemyInformation : public PolymorphicComparable<Component, EnemyInformation>
	{
	public:
		EnemyInformation(float health) : health(health), maxHealth(health)
		{}


		float health;
		float maxHealth;

	};
}
