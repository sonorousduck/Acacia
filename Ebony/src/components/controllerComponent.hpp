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

		std::unordered_map<int, std::string_view> bindings{}; // This is what defines our controller bindings. i.e. pressing space gives the string_view "jump"
		std::unordered_map<std::string_view, std::function<void()>> onPressActions{}; // This defines our on initial press actions. i.e. "jump" causes the player to... jump...
		std::unordered_map<std::string_view, std::function<void()>> onHeldActions{}; // This defines our on held actions. i.e. holding w keeps going forward
		std::unordered_map<std::string_view, std::function<void()>> onReleaseActions{}; // This defines our on released actions. i.e. releasing at the correct time to time something


		//std::unordered_map<int, std::function<void()>> controllerActionKeyPairs{};
		std::unordered_map<int, std::string_view> joystickBindings{};
		std::unordered_map<std::string_view, std::function<void(float)>> joystickActions{};

		//std::unordered_map<int, bool> actions{};
		//std::unordered_map<int, bool> previousActions{};

		// TODO: Probably need to have a controller id attributed to an input as well (i.e. so you can have it known this is player one's input)
		int joystickId = -1;
	};
}