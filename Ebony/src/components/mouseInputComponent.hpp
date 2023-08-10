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

		std::unordered_map<int, bool> actions{};
		std::unordered_map<int, bool> previousActions{};
	};
}