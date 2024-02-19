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

					state.playerPosition.box = { transform->position.x - Ebony::Graphics2d::mainCamera->Position.x, transform->position.y - Ebony::Graphics2d::mainCamera->Position.y, transform->rotation / 360.0f, state.playerPosition.box[3] };

					break;
				}
				case (SpaceGuy::ENEMY_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();


					if ((Ebony::Graphics2d::mainCamera->Position.x - 75 < transform->position.x && Ebony::Graphics2d::mainCamera->Position.x + 480 > transform->position.x &&
						Ebony::Graphics2d::mainCamera->Position.y - 75 < transform->position.y && Ebony::Graphics2d::mainCamera->Position.y + 320 > transform->position.y))
					{
						state.enemyPositions.push_back(Ebony::Box(transform->position.x - Ebony::Graphics2d::mainCamera->Position.x, transform->position.y - Ebony::Graphics2d::mainCamera->Position.y, 0.0f, 0.0f));
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
						state.bulletInformation.push_back(Ebony::Box(transform->position.x - Ebony::Graphics2d::mainCamera->Position.x, transform->position.y - Ebony::Graphics2d::mainCamera->Position.y, velocity.x, velocity.y));
					}
					break;
				}
				case (SpaceGuy::KEY_INFORMATION):
				{
					auto transform = entity->getComponent<components::Transform>();

					state.keyPosition.box = { transform->position.x - Ebony::Graphics2d::mainCamera->Position.x, transform->position.y - Ebony::Graphics2d::mainCamera->Position.y, 0.0f, 0.0f };

					break;
				}
				}
			}

			if (aiComponent->aiType & Ebony::AIType::REWARD)
			{
				if (aiComponent->information & SpaceGuy::PLAYER_INFORMATION)
				{
					auto player = entity->getComponent<components::PlayerInformation>();

					reward.n += static_cast<int>(player->score);
				}
			}
		}



		SpaceGuy::SpaceGuyPythonManager::state = state;
		SpaceGuy::SpaceGuyPythonManager::reward = reward;

	}

}