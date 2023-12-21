#pragma once

#include <glm/glm.hpp>
#include <components/component.hpp>

namespace components
{
	class Crosshair : public PolymorphicComparable<Component, Crosshair>
	{
	public:
		Crosshair()
		{}


		glm::vec2 aimDirection{};
		glm::vec2 aimLocation{};
	};
}
