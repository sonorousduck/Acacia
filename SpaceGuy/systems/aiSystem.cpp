#include "aiSystem.hpp"
#include <components/rigidbodyComponent.hpp>
#include "../components/playerInformation.hpp"


namespace systems
{
	void AISystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void AISystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		auto state = SpaceGuy::State();
		auto reward = Ebony::Discrete();


		for (auto& [id, entity] : m_Entities)
		{
			auto aiComponent = entity->getComponent<components::AIComponent>();

			if (aiComponent->aiType & Ebony::AIType::STATE)
			{
				switch (aiComponent->information)
				{
				case (SpaceGuy::PLAYER_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();

					state.playerPosition.box = { transform->position.x, transform->position.y, transform->rotation / 360.0f, state.playerPosition.box[3] };

					break;
				}
				case (SpaceGuy::ENEMY_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();


					if ((Ebony::Graphics2d::mainCamera->Position.x - 75 < transform->position.x && Ebony::Graphics2d::mainCamera->Position.x + 480 > transform->position.x &&
						Ebony::Graphics2d::mainCamera->Position.y - 75 < transform->position.y && Ebony::Graphics2d::mainCamera->Position.y + 320 > transform->position.y))
					{
						state.enemyPositions.push_back(Ebony::Box(transform->position.x, transform->position.y, 0.0f, 0.0f));
					}
					break;
				}
				case (SpaceGuy::ENEMY_BULLET_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();
					auto rigidBody = entity->getComponent<components::RigidBody>();
					auto velocity = rigidBody->getVelocity();

					if ((Ebony::Graphics2d::mainCamera->Position.x - 75 < transform->position.x && Ebony::Graphics2d::mainCamera->Position.x + 480 > transform->position.x &&
						Ebony::Graphics2d::mainCamera->Position.y - 75 < transform->position.y && Ebony::Graphics2d::mainCamera->Position.y + 320 > transform->position.y))
					{
						state.bulletInformation.push_back(Ebony::Box(transform->position.x, transform->position.y, velocity.x, velocity.y));
					}
					break;
				}
				case (SpaceGuy::KEY_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();

					state.keyPosition.box = { transform->position.x, transform->position.y, 0.0f, 0.0f };

					break;
				}
				}
			}

			if (aiComponent->aiType & Ebony::AIType::REWARD)
			{
				if (aiComponent->information & SpaceGuy::PLAYER_INFORMATION)
				{
					auto player = entity->getComponent<components::PlayerInformation>();
					auto transform = entity->getComponent<components::Transform>();

					if (player->hasKey)
					{
						reward.n += std::min(1.0 / glm::distance(transform->position, glm::vec2(2768.0f, 448.0f)) * 10000.0, 10000.0);
						reward.n += 10000.0f; // Reward the player for grabbing the key, so they don't just stay really close to the key for the reward
					}
					else
					{
						reward.n += std::min((1.0f / (glm::distance(transform->position, glm::vec2(97.0f, 528.0f)) + 0.01)) * 10000.0, 10000.0);;
					}

					float punishment = Ebony::Time::GetTotalElapsedTime();
					reward.n += static_cast<int>(player->score - punishment);
				}
			}
		}



		SpaceGuy::SpaceGuyPythonManager::state = state;
		SpaceGuy::SpaceGuyPythonManager::reward = reward;

	}

}