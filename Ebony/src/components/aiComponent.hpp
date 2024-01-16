#pragma once

#include "component.hpp"
#include "../Core.hpp"


namespace Ebony
{
	enum AIType : std::uint8_t
	{
		STATE = BIT(0),
		REWARD = BIT(1)
	};
}


namespace components
{
	class AIComponent : public PolymorphicComparable<Component, AIComponent>
	{
	public:
		AIComponent(std::uint8_t aiType, std::uint64_t information) : aiType(aiType), information(information)
		{
		}

		std::uint8_t aiType;
		std::uint64_t information;
	};
}