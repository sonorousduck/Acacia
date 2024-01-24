#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

#include <Core.hpp>

namespace components
{
	class Bullet : public PolymorphicComparable<Component, Bullet>
	{
	public:
		Bullet(float strength = 1.0f) : strength(strength)
		{}


		glm::vec2 velocity{};
		float strength;

	};
}
