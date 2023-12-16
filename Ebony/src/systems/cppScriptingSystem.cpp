#include "cppScriptingSystem.hpp"
#include <iostream>


namespace systems
{
	void CppScriptingSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void CppScriptingSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto script = entity->getComponent<components::CppScript>();


			script->Update(elapsedTime);
		}

	}
}

