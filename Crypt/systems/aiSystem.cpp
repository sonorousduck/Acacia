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

		auto state = Crypt::State();
		auto reward = Ebony::Discrete();

		for (auto& [id, entity] : m_Entities)
		{
			auto& test = entity->getComponents();
			auto aiComponent = entity->getComponent<components::AIComponent>();

			if (aiComponent->aiType & Ebony::AIType::STATE)
			{
				switch (aiComponent->information)
				{
				case (Crypt::PLAYER_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>()->position;
					state.playerPosition.box = { transform.x - Ebony::Graphics2d::mainCamera->Position.x, transform.y - Ebony::Graphics2d::mainCamera->Position.y, state.playerPosition.box[2], state.playerPosition.box[3] };
					break;
				}

				case (Crypt::CROSSHAIR_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>()->position;

					state.playerPosition.box = { state.playerPosition.box[0], state.playerPosition.box[1], transform.x - Ebony::Graphics2d::mainCamera->Position.x, transform.y - Ebony::Graphics2d::mainCamera->Position.y };
					break;
				}
				case (Crypt::ENEMY_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();


					if ((Ebony::Graphics2d::mainCamera->Position.x - 75 < transform->position.x && Ebony::Graphics2d::mainCamera->Position.x + 480 > transform->position.x &&
						Ebony::Graphics2d::mainCamera->Position.y - 75 < transform->position.y && Ebony::Graphics2d::mainCamera->Position.y + 320 > transform->position.y))
					{
						state.enemyPositions.push_back(Ebony::Box(transform->position.x - Ebony::Graphics2d::mainCamera->Position.x, transform->position.y - Ebony::Graphics2d::mainCamera->Position.y, 0.0f, 0.0f));
					}
					break;
				}

				case (Crypt::BULLET_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();
					auto rigidBody = entity->getComponent<components::RigidBody>();
					auto velocity = rigidBody->getVelocity();

					if ((Ebony::Graphics2d::mainCamera->Position.x - 75 < transform->position.x && Ebony::Graphics2d::mainCamera->Position.x + 480 > transform->position.x &&
						Ebony::Graphics2d::mainCamera->Position.y - 75 < transform->position.y && Ebony::Graphics2d::mainCamera->Position.y + 320 > transform->position.y))
					{
						state.bulletInformation.push_back(Ebony::Box(transform->position.x - Ebony::Graphics2d::mainCamera->Position.x, transform->position.y - Ebony::Graphics2d::mainCamera->Position.y, velocity.x, velocity.y));
					}

					break;
				}

				default:
					break;
				}
			}
			
			if (aiComponent->aiType & Ebony::AIType::REWARD)
			{

				if (aiComponent->information & Crypt::PLAYER_INFORMATION)
				{
					// Score
					// Lives

					auto player = entity->getComponent<components::Player>();
					//auto health = player->health;
					auto score = player->score / 100;
					//reward.n += health * 10 + static_cast<int>(score);

					reward.n += static_cast<int>(score);
				}



				// This is the reward information
			}


			// After compiling it together, give it to python
			


			// If a component has an AI Input, then the response from the python code will be given to the AI Input here

		}

		Crypt::CryptPythonManager::state = state;
		Crypt::CryptPythonManager::reward = reward;

	}

}