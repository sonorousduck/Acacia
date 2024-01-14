#include "pythonManager.hpp"


namespace Ebony
{
	std::vector<std::vector<std::string>> PythonManager::actions{};
	bool PythonManager::initialized{ false };
	pybind11::module PythonManager::pyModule;


	void PythonManager::Update(std::chrono::microseconds elapsedTime)
	{
		PythonManager::pyModule.attr("Update")();
	}
	void PythonManager::ProcessInput()
	{
	}
	void PythonManager::Init(const char* filename)
	{
		pybind11::initialize_interpreter();

		PythonManager::pyModule = pybind11::module_::import(filename);
		PythonManager::initialized = true;

		PythonManager::pyModule.attr("Start")();
	}


	void PythonManager::Destroy()
	{
		if (PythonManager::initialized)
		{
			PythonManager::pyModule.release();
			pybind11::finalize_interpreter();
		}
	}
}