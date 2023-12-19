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
		Transform(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f)) :
			position(position),
			rotation(rotation),
			scale(scale),
			rotationAxis(rotationAxis)
		{}

		void SetPosition(glm::vec2 newPosition)
		{
			previousPosition = position;
			position = newPosition;
		}

		glm::vec2 position;
		glm::vec2 previousPosition{};
		float rotation;
		glm::vec3 rotationAxis;
		glm::vec2 scale;
	};
}