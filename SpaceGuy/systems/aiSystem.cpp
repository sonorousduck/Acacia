#include "aiSystem.hpp"
#include <components/rigidbodyComponent.hpp>

namespace systems
{
	void AISystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void AISystem::updateImpl(std::chrono::microseconds elapsedTime)
	{

		

	}

}