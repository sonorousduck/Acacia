#pragma once

#include <functional>

#include "component.hpp"


namespace components
{
	class AiInput : public PolymorphicComparable<Component, AiInput>
	{
	public:
		AiInput() {};

		std::unordered_map<std::string, std::function<void()>> actions{};
		// This maps states to actions.
		std::function<void()> translationFunction;

	};
}