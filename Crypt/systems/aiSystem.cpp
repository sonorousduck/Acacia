#include "aiSystem.hpp"

namespace systems
{
	void AISystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void AISystem::updateImpl(std::chrono::microseconds elapsedTime)
	{

		auto state = Crypt::State();
		auto reward = Ebony::Discrete();

		for (auto& [id, entity] : m_Entities)
		{
			auto& test = entity->getComponents();
			auto aiComponent = entity->getComponent<components::AIComponent>();

			if (aiComponent->aiType & Ebony::AIType::STATE)
			{
				// Then the information will go here
				if (aiComponent->information & Crypt::PLAYER_INFORMATION)
				{
					auto transform = entity->getComponent<components::Transform>()->position;
					state.playerPosition.box = {transform.x, transform.y, 0.0f, 0.0f};
				}
			}
			
			if (aiComponent->aiType & Ebony::AIType::REWARD)
			{

				if (aiComponent->information & Crypt::PLAYER_INFORMATION)
				{
					// Score
					// Lives

					auto player = entity->getComponent<components::Player>();
					auto health = player->health;
					auto score = player->score / 100;

					reward.n += health * 3 + score;
				}



				// This is the reward information
			}


			// After compiling it together, give it to python
			


			// If a component has an AI Input, then the response from the python code will be given to the AI Input here

		}

		Crypt::CryptPythonManager::states[0].push_back(state);
		Crypt::CryptPythonManager::rewards[0].push_back(reward);

	}

}