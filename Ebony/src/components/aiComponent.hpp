#pragma once

#include "component.hpp"
#include "../Log.hpp"


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
		AIComponent(Ebony::AIType aiType) : aiType(aiType)
		{
		}

		Ebony::AIType aiType;
	};
}