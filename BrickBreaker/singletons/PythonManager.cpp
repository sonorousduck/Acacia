#include "PythonManager.hpp"


namespace BrickBreaker
{
	int PythonManager::action{};
	State PythonManager::state{};
	Ebony::Discrete PythonManager::reward{};

	bool PythonManager::initialized{ false };
	pybind11::module PythonManager::pyModule;
	

	PYBIND11_EMBEDDED_MODULE(cpp_module, m)
	{
		pybind11::class_<State>(m, "State")
			.def(pybind11::init<Ebony::Box>())
			.def("getBrickPositions", &State::getBrickPositions)
			.def("getPowerupPositions", &State::getPowerupPositions)
			.def("getPaddlePosition", &State::getPaddlePosition)
			.def("getBallPosition", &State::getBallPosition);

		pybind11::class_<Ebony::Box>(m, "Box")
			.def(pybind11::init())
			.def("setBox", &Ebony::Box::setBox)
			.def("getBox", &Ebony::Box::getBox)
			;

		pybind11::class_<Ebony::Discrete>(m, "Discrete")
			.def(pybind11::init())
			.def("setValue", &Ebony::Discrete::setValue)
			.def("getValue", &Ebony::Discrete::getValue);

		//pybind11::class_<Ebony::Discrete>(m, "Reward")
		//	.def(pybind11::init())
		//	.def("getReward", &Ebony::Discrete::getValue);
	}



	void PythonManager::Update(std::chrono::microseconds elapsedTime)
	{

	}

	void PythonManager::Reset()
	{
			//CryptPythonManager::actions.clear();
		auto& state = PythonManager::state;;
		auto& reward = PythonManager::reward;
		PythonManager::pyModule.attr("Reset")(state, reward);
	}


	void PythonManager::ProcessInput()
	{
	}
	void PythonManager::Init(const char* filename, bool isAiStartup)
	{
		pybind11::initialize_interpreter();



		PythonManager::pyModule = pybind11::module_::import(filename);
		//CryptPythonManager::stateModule = pybind11::module_::import("cpp_module");
		PythonManager::initialized = true;

		if (isAiStartup)
		{
			PythonManager::pyModule.attr("StartGames")();
		}

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