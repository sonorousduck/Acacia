#include "aiSystem.hpp"

// NOTE: THESE ARE PLACEHOLDERS TO USE IN INDIVIDUAL GAMES! SINCE STATE IS DEPENDENT ON THE GAME
// YOU CAN'T HAVE A GENERALIZED AI SYSTEM

namespace systems
{
	void BaseAISystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void BaseAISystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			if (!entity->isEnabled()) return;


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