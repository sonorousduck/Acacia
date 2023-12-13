#include "aiSystem.hpp"

namespace systems
{
	void AISystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void AISystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto& test = entity->getComponents();
			auto aiComponent = entity->getComponent<components::AIComponent>();

			if (aiComponent->aiType & Ebony::AIType::STATE)
			{
				// Then the information will go here
			}
			else if (aiComponent->aiType & Ebony::AIType::REWARD)
			{
				// This is the reward information
			}


			// After compiling it together, give it to python
			


			// If a component has an AI Input, then the response from the python code will be given to the AI Input here

		}
	}

}