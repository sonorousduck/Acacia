#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

#include <Core.hpp>

namespace components
{

	enum BULLET_TYPE : std::uint8_t
	{
		FIRE = BIT(0),
		ICE = BIT(1)
	};


	class Bullet : public PolymorphicComparable<Component, Bullet>
	{
	public:
		Bullet(BULLET_TYPE bulletType, std::uint8_t strength = 1) : bulletType(bulletType), strength(strength)
		{}


		glm::vec2 velocity{};
		std::uint8_t strength;
		BULLET_TYPE bulletType;

	};
}
