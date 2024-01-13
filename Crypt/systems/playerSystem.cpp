#include "playerSystem.hpp"
#include <components/rigidbodyComponent.hpp>
#include <components/transform.hpp>


namespace systems
{
	void PlayerSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void PlayerSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto player = entity->getComponent<components::Player>();
			auto rigidBody = entity->getComponent<components::RigidBody>();
			auto transform = entity->getComponent<components::Transform>();
			

			player->score += transform->position.x - transform->previousPosition.x;
			/*if (player->isSlow)
			{
				if (player->switchedSpeeds)
				{
					player->smoothingTime = 0.0f;
					player->switchedSpeeds = false;
				}

				if (player->smoothingTime < player->totalCameraSmoothingTime)
				{
					player->smoothingTime += elapsedTime.count() / 1000000.0f;

					Ebony::Graphics2d::mainCamera->SetCameraXPositionSmooth(transform->position.x - 30.0f, player->smoothingTime, player->totalCameraSmoothingTime);
				}


			}
			else if (player->isFast)
			{
				if (player->switchedSpeeds)
				{
					player->smoothingTime = 0.0f;
					player->switchedSpeeds = false;
				}

				if (player->smoothingTime < player->totalCameraSmoothingTime)
				{
					player->smoothingTime += elapsedTime.count() / 1000000.0f;
					Ebony::Graphics2d::mainCamera->SetCameraXPositionSmooth(transform->position.x - 100.0f, player->smoothingTime, player->totalCameraSmoothingTime);
				}

			}
			else
			{
				if (player->smoothingTime < player->returnSmoothingTime)
				{
					if (!player->switchedSpeeds)
					{
						player->returnSmoothingTime = player->totalCameraSmoothingTime - player->smoothingTime;
						player->smoothingTime = 0.0f;
					}
					player->smoothingTime += elapsedTime.count() / 1000000.0f;
					Ebony::Graphics2d::mainCamera->SetCameraXPositionSmooth(transform->position.x - 70.0f, player->smoothingTime, player->returnSmoothingTime);
					player->switchedSpeeds = true;
				}
				else
				{
					Ebony::Graphics2d::mainCamera->SetCameraXPosition(transform->position.x - 70.0f);
					player->switchedSpeeds = false;

				}
			}*/

			Ebony::Graphics2d::mainCamera->SetCameraXPosition(transform->position.x - 100.0f);


			if (player->isOnGround)
			{
				rigidBody->setVelocity(glm::vec2(rigidBody->getVelocity().x, 0.0f));
				rigidBody->setAcceleration(glm::vec2(rigidBody->getAcceleration().x, 0.0f));
			}

			if (player->gravityCooldown > 0.0f)
			{
				player->gravityCooldown -= elapsedTime.count() / 1000000.0f;
			}

		}
	}
}

