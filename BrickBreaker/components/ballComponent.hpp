#pragma once

#include <glm/glm.hpp>

#include <components/component.hpp>


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



	};
}
