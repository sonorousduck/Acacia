#include "SpaceGuyPythonManager.hpp"


namespace SpaceGuy
{
	Ebony::Action SpaceGuyPythonManager::action{};
	State SpaceGuyPythonManager::state{};
	Ebony::Discrete SpaceGuyPythonManager::reward{};

	bool SpaceGuyPythonManager::initialized{ false };
	pybind11::module SpaceGuyPythonManager::pyModule;
	//pybind11::module SpaceGuyPythonManager::stateModule;

	/*PYBIND11_EMBEDDED_MODULE(cpp_module, m)
	{
		pybind11::class_<State> state(m, "State");
	}*/

	PYBIND11_EMBEDDED_MODULE(cpp_module, m)
	{
		pybind11::class_<State>(m, "State")
			.def(pybind11::init<Ebony::Box>())
			.def("getPlayerPosition", &State::getPlayerPosition)
			.def("getEnemyPositions", &State::getEnemyPositions)
			.def("getBulletInformation", &State::getBulletInformation)
			.def("getKeyInformation", &State::getKeyInformation)
			.def("__repr__", [](const State& state) { return "<State player x position " + std::to_string(state.playerPosition.box.at(0)) + ">"; });

		pybind11::class_<Ebony::Box>(m, "Box")
			.def(pybind11::init())
			.def("setBox", &Ebony::Box::setBox)
			.def("getBox", &Ebony::Box::getBox)
			;

		pybind11::class_<Ebony::Action>(m, "Action")
			.def(pybind11::init())
			.def("setAction", &Ebony::Action::setAction)
			.def("getAction", &Ebony::Action::getAction)
			;

		pybind11::class_<Ebony::Discrete>(m, "Discrete")
			.def(pybind11::init())
			.def("setValue", &Ebony::Discrete::setValue)
			.def("getValue", &Ebony::Discrete::getValue);
	}



	void SpaceGuyPythonManager::Update(std::chrono::microseconds elapsedTime)
	{
		
	}

	void SpaceGuyPythonManager::Reset()
	{
		auto& state = SpaceGuyPythonManager::state;;
		auto& reward = SpaceGuyPythonManager::reward;
		SpaceGuyPythonManager::pyModule.attr("Reset")(state, reward);
	}


	void SpaceGuyPythonManager::ProcessInput()
	{
	}
	void SpaceGuyPythonManager::Init(const char* filename, bool isAiStartup)
	{
		pybind11::initialize_interpreter();



		SpaceGuyPythonManager::pyModule = pybind11::module_::import(filename);
		//SpaceGuyPythonManager::stateModule = pybind11::module_::import("cpp_module");
		SpaceGuyPythonManager::initialized = true;

		if (isAiStartup)
		{
			SpaceGuyPythonManager::pyModule.attr("StartGames")();
		}
	}


	void SpaceGuyPythonManager::Destroy()
	{
		if (SpaceGuyPythonManager::initialized)
		{
			SpaceGuyPythonManager::pyModule.release();
			pybind11::finalize_interpreter();
		}
	}
}