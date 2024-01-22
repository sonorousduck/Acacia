#include "aiInputSystem.hpp"


namespace systems
{
	void AIInputSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}
	void AIInputSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			if (!entity->isEnabled()) return;

			auto aiInput = entity->getComponent<components::AiInput>();
			aiInput->translationFunction();
		}


	}
}