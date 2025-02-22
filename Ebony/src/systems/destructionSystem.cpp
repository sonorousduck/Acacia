#include "destructionSystem.hpp"

namespace systems
{
	void DestructionSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void DestructionSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			if (!entity->isEnabled()) return;


			auto destructionComponent = entity->getComponent<components::DestructionComponent>();

			if (destructionComponent->shouldDestroy)
			{
				destructionComponent->onDestroyCallback();
			}

			if (destructionComponent->shouldRemove)
			{
				Ebony::SystemManager::RemoveEntity(id);
			}


		}
	}

}