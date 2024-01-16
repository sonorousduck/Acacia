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
			auto aiInput = entity->getComponent<components::AiInput>();
			aiInput->translationFunction();
		}


	}
}