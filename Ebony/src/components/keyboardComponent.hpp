#pragma once

#include <functional>
#include "component.hpp"


namespace components
{
	// I think that derived classes could be handled PolymorphicComparable<Input, MouseInput> or something like that. That way they will all show up under input
	class KeyboardInput : public PolymorphicComparable<Component, KeyboardInput>
	{
	public:
		KeyboardInput() {};

		std::unordered_map<int, std::function<void()>> keyboardActionKeyPairs{};
		std::unordered_map<int, std::function<void()>> onReleaseKeyboardActionKeyPairs{};

		std::unordered_map<int, bool> actions{};
		std::unordered_map<int, bool> previousActions{};
	};
}