#pragma once

#include <functional>
#include "component.hpp"


namespace components
{
	class Input : public PolymorphicComparable<Component, Input>
	{
	public:
		Input() {};
		Input(int joystickId) : joystickId(joystickId) {};

		std::unordered_map<int, std::function<void()>> controllerActionKeyPairs{};
		std::unordered_map<int, std::function<void(float)>> controllerAxes{};

		std::unordered_map<int, std::function<void()>> keyboardActionKeyPairs{};
		std::unordered_map<int, std::function<void()>> onReleaseKeyboardActionKeyPairs{};

		std::unordered_map<int, bool> actions{};
		std::unordered_map<int, bool> previousActions{};

		// TODO: Probably need to have a controller id attributed to an input as well (i.e. so you can have it known this is player one's input)
		int joystickId = -1;
	};
}