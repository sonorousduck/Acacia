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
		Bullet(std::uint8_t strength = 1) : strength(strength)
		{}


		glm::vec2 velocity{};
		std::uint8_t strength;

	};
}
