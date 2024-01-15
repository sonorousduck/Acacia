#include "CryptPythonManager.hpp"


namespace Crypt
{
	std::vector<std::vector<Ebony::Box>> CryptPythonManager::actions{};
	std::vector<std::vector<State>> CryptPythonManager::states{};
	bool CryptPythonManager::initialized{ false };
	pybind11::module CryptPythonManager::pyModule;
	//pybind11::module CryptPythonManager::stateModule;

	/*PYBIND11_EMBEDDED_MODULE(cpp_module, m)
	{
		pybind11::class_<State> state(m, "State");
	}*/

	PYBIND11_EMBEDDED_MODULE(cpp_module, m)
	{
		pybind11::class_<State>(m, "State")
			.def(pybind11::init<Ebony::Box>())
			.def("getPlayerPosition", &State::getPlayerPosition)
			.def("testPlayerPosition", &State::testPlayerPosition)
			.def("__repr__", [](const State& state) { return "<cpp_module.state position " + std::to_string(state.playerPosition.box.at(0)) + ">"; });

		pybind11::class_<Ebony::Box>(m, "Action")
			.def(pybind11::init())
			.def("setBox", &Ebony::Box::setBox);
	}



	void CryptPythonManager::Update(std::chrono::microseconds elapsedTime)
	{
		auto test = State();
		test.playerPosition = Ebony::Box(1.0f, 2.0f, 3.0f, 4.0f);


		pybind11::object result = CryptPythonManager::pyModule.attr("Update")(test);
		auto test1 = result.cast<Ebony::Box>();


		std::cout << "Result" << std::endl;
	}
	void CryptPythonManager::ProcessInput()
	{
	}
	void CryptPythonManager::Init(const char* filename, std::uint64_t environmentCount)
	{
		pybind11::initialize_interpreter();



		CryptPythonManager::pyModule = pybind11::module_::import(filename);
		//CryptPythonManager::stateModule = pybind11::module_::import("cpp_module");
		CryptPythonManager::initialized = true;

		CryptPythonManager::states.resize(environmentCount);
		CryptPythonManager::actions.resize(environmentCount);


		CryptPythonManager::pyModule.attr("Start")();
	}


	void CryptPythonManager::Destroy()
	{
		if (CryptPythonManager::initialized)
		{
			CryptPythonManager::pyModule.release();
			pybind11::finalize_interpreter();
		}
	}
}