#include "pythonScriptingSystem.hpp"



namespace systems
{
	void PythonScriptingSystem::Start()
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto script = entity->getComponent<components::PythonScript>();			
		}
	}


	void PythonScriptingSystem::Update(std::chrono::microseconds elapsedTime)
	{
		if (!hasStarted)
		{
			Start();
			hasStarted = true;
		}

		this->updateImpl(elapsedTime);
	}

	void PythonScriptingSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			//auto test = 

			auto script = entity->getComponent<components::PythonScript>();
			//auto pyModule = pybind11::module_::import(script->filename.c_str());

			//pyModule.attr("Update")();

			
		}

	}


}

