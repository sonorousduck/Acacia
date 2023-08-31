#pragma once

#include <functional>

#include "component.hpp"
#include "inputComponent.hpp"

namespace components
{
	class MouseInput : public PolymorphicComparable<Input, MouseInput>
	{
	public:
		MouseInput() {};

		float positionX = 0;
		float positionY = 0;
		float previousX = 0;
		float previousY = 0;


		std::unordered_map<int, std::function<void()>> actions{}; // This is for buttons on the mouse
		std::unordered_map<int, std::function<void()>> previousActions{};
	};
}