#include "cppScriptingSystem.hpp"


namespace systems
{
	void CppScriptingSystem::Start()
	{
		for (auto& [id, entity] : m_Entities)
		{
			entity->getComponent<components::CppScript>()->Start();
		}
	}


	void CppScriptingSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void CppScriptingSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			//auto test = 

			auto script = entity->getComponent<components::CppScript>();


			script->Update(elapsedTime);
		}

	}


}

