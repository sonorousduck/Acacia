#pragma once

#include <functional>

#include "component.hpp"

namespace components
{
	class MouseInput : public PolymorphicComparable<Component, MouseInput>
	{
	public:
		MouseInput() {};

		float positionX = 0;
		float positionY = 0;
		float previousX = 0;
		float previousY = 0;



		std::unordered_map<int, std::string_view> bindings{}; // This is what defines our key bindings. i.e. pressing space gives the string_view "jump"
		std::unordered_map<std::string_view, std::function<void()>> onPressActions{}; // This defines our on initial press actions. i.e. "jump" causes the player to... jump...
		std::unordered_map<std::string_view, std::function<void()>> onHeldActions{}; // This defines our on held actions. i.e. holding w keeps going forward
		std::unordered_map<std::string_view, std::function<void()>> onReleaseActions{}; // This defines our on released actions. i.e. releasing at the correct time to time something
	};
}