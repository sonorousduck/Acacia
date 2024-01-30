#include "aiSystem.hpp"
#include <components/rigidbodyComponent.hpp>
#include "../singletons/GameManager.hpp"

namespace systems
{
	void AISystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void AISystem::updateImpl(std::chrono::microseconds elapsedTime)
	{

		auto state = BrickBreaker::State();
		auto reward = Ebony::Discrete();

		for (auto& [id, entity] : m_Entities)
		{
			auto& test = entity->getComponents();
			auto aiComponent = entity->getComponent<components::AIComponent>();

			if (aiComponent->aiType & Ebony::AIType::STATE)
			{
			 	switch (aiComponent->information)
				{
				case (BrickBreaker::PADDLE_INFORMATION):
			 	{
			 		auto transform = entity->getComponent<components::Transform>()->position;
			 		state.paddlePosition.box = { transform.x, transform.y, 0.0f, 0.0f };
			 		break;
			 	}

				case (BrickBreaker::BALL_INFORMATION):
			 	{
			 		auto transform = entity->getComponent<components::Transform>()->position;

			 		state.ballPosition.box = { transform.x, transform.y, 0.0f, 0.0f };
			 		break;
			 	}
			 	case (BrickBreaker::BRICK_INFORMATION):
			 	{
			 		auto transform = entity->getComponent<components::Transform>();

					if (state.brickPositions.size() < 5)
					{
			 			state.brickPositions.push_back(Ebony::Box(transform->position.x, transform->position.y, 0.0f, 0.0f));
					}

			 		
			 		break;
			 	}

			 	case (BrickBreaker::POWERUP_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();
					auto rigidBody = entity->getComponent<components::RigidBody>();
					auto velocity = rigidBody->getVelocity();

					if (state.powerupPositions.size() < 2)
					{
						state.powerupPositions.push_back(Ebony::Box(transform->position.x, transform->position.y, velocity.x, velocity.y));
					}

			 		break;
			 	}

			 	default:
			 		break;
			 	}
			}
			
			if (aiComponent->aiType & Ebony::AIType::REWARD)
			{
				reward.n = static_cast<int>(BrickBreaker::GameManager::getPoints());

				// if (aiComponent->information & Crypt::PLAYER_INFORMATION)
				// {
				// 	// Score
				// 	// Lives

				// 	auto player = entity->getComponent<components::Player>();
				// 	auto health = player->health;
				// 	auto score = player->score / 100;

				// 	reward.n += health * 10 + static_cast<int>(score);
				// }



				// This is the reward information
			}


			// After compiling it together, give it to python
			


			// If a component has an AI Input, then the response from the python code will be given to the AI Input here

		}

		BrickBreaker::PythonManager::state = state;
		BrickBreaker::PythonManager::reward = reward;

	}

}