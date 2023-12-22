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
			auto destructionComponent = entity->getComponent<components::DestructionComponent>();

			if (destructionComponent->shouldDestroy)
			{
				destructionComponent->onDestroyCallback();
			}

			if (destructionComponent->shouldRemove)
			{
				OnRemove(id);
			}


		}
	}

}