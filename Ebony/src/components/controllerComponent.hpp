#pragma once

#include <functional>
#include "component.hpp"


namespace components
{
	// I think that derived classes could be handled PolymorphicComparable<Input, MouseInput> or something like that. That way they will all show up under input
	class ControllerInput : public PolymorphicComparable<Component, ControllerInput>
	{
	public:
		ControllerInput() {};
		ControllerInput(int joystickId) : joystickId(joystickId) {};

		std::unordered_map<int, std::function<void()>> controllerActionKeyPairs{};
		std::unordered_map<int, std::function<void(float)>> controllerAxes{};

		std::unordered_map<int, bool> actions{};
		std::unordered_map<int, bool> previousActions{};

		// TODO: Probably need to have a controller id attributed to an input as well (i.e. so you can have it known this is player one's input)
		int joystickId = -1;
	};
}