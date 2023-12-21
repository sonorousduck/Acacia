#include "bulletSystem.hpp"
#include <components/rigidbodyComponent.hpp>
#include <components/transform.hpp>


namespace systems
{
	void BulletSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void BulletSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto player = entity->getComponent<components::Bullet>();
			auto rigidBody = entity->getComponent<components::RigidBody>();
			auto transform = entity->getComponent<components::Transform>();

			
		}
	}
}

