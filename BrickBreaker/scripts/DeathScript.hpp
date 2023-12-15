#pragma once

#include <iostream>
#include "../Ebony/src/components/cppScriptComponent.hpp"

namespace components
{
	class DeathScript : public PolymorphicComparableInherited<Component, CppScriptBase, DeathScript>
	{
	public:
		DeathScript()
		{

		}

		void Update(std::chrono::microseconds elapsedTime)
		{
			std::cout << "Calling update!" << std::endl;
		}

	};
}