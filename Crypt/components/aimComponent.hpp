#pragma once

#include <glm/glm.hpp>
#include <components/component.hpp>
#include "../components/bulletComponent.hpp"

namespace components
{
	class Crosshair : public PolymorphicComparable<Component, Crosshair>
	{
	public:
		Crosshair()
		{}


		glm::vec2 aimDirection{};
		glm::vec2 aimLocation{};
		components::BULLET_TYPE bulletType = components::BULLET_TYPE::FIRE;
	};
}
