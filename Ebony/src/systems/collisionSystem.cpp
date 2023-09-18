#include "collisionSystem.hpp"

namespace systems
{
	void CollisionSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void CollisionSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto collider = entity->getComponent<components::Collider>();





		}
	}
}