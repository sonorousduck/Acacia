#include "playerSystem.hpp"
#include <components/rigidbodyComponent.hpp>


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


			if (player->isOnGround)
			{
				rigidBody->setVelocity(glm::vec2(rigidBody->getVelocity().x, 0.0f));
				rigidBody->setAcceleration(glm::vec2(rigidBody->getAcceleration().x, 0.0f));
			}

		}
	}
}

