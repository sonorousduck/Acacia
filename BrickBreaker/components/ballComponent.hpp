#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

namespace components
{
	class Ball : public PolymorphicComparable<Component, Ball>
	{
	public:
		Ball(float speed, glm::vec2 direction, float strength, bool isAttachedToPaddle = false) : 
			speed(speed),
			direction(direction),
			strength(strength), 
			isAttachedToPaddle(isAttachedToPaddle)
		{}

		float speed;
		glm::vec2 direction;
		float strength;
		bool isAttachedToPaddle; // This is for when you start a round and have to shoot the ball off

		inline double random_double(double start, double end)
		{
			static std::uniform_real_distribution<double> distribution(start, end);
			static std::mt19937 generator;
			return distribution(generator);
		}

		inline glm::vec2 random_double_vec2(double start, double end)
		{
			static std::uniform_real_distribution<double> distribution(start, end);
			static std::mt19937 generator;
			return glm::vec2(distribution(generator), distribution(generator));
		}

	};
}
