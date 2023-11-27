#pragma once

#include "component.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace components
{
	class Transform : public PolymorphicComparable<Component, Transform>
	{
	public:
		Transform(glm::vec2 position, float rotation, glm::vec2 scale) :
			position(position),
			rotation(rotation),
			scale(scale)
		{}

		glm::vec2 position;
		glm::vec2 previousPosition{};
		float rotation;
		glm::vec2 scale;
	};
}