#include "pythonManager.hpp"


namespace Ebony
{
	//std::vector<std::vector<std::string>> PythonManager::actions{};
	//std::vector<std::vector<std::string>> PythonManager::states{};

	bool PythonManager::initialized{ false };
	pybind11::module PythonManager::pyModule;


	void PythonManager::Update(std::chrono::microseconds elapsedTime)
	{
		pybind11::object result = PythonManager::pyModule.attr("Update")(Ebony::Time::GetDeltaTimeFloat());
		pybind11::print(pybind11::int_(result));
		std::cout << "Result" << std::endl;
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