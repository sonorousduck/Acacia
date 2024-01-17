#include "EcologyPythonManager.hpp"


namespace EcologyRTS
{
	std::vector<std::vector<Ebony::Box>> EcologyPythonManager::actions{};
	std::vector<std::vector<State>> EcologyPythonManager::states{};
	std::vector<std::vector<Ebony::Discrete>> EcologyPythonManager::rewards{};

	bool EcologyPythonManager::initialized{ false };
	pybind11::module EcologyPythonManager::pyModule;
	//pybind11::module EcologyPythonManager::stateModule;

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
			.def("__repr__", [](const State& state) { return "<State player x position " + std::to_string(state.playerPosition.box.at(0)) + ">"; });

		pybind11::class_<Ebony::Box>(m, "Box")
			.def(pybind11::init())
			.def("setBox", &Ebony::Box::setBox)
			.def("getBox", &Ebony::Box::getBox)
			;

		pybind11::class_<Ebony::Discrete>(m, "Reward")
			.def(pybind11::init())
			.def("getReward", &Ebony::Discrete::getValue);
	}



	void EcologyPythonManager::Update(std::chrono::microseconds elapsedTime)
	{
		for (auto i = 0; i < EcologyPythonManager::states.size(); i++)
		{
			if (EcologyPythonManager::states[i].size() != 0)
			{
				EcologyPythonManager::actions[i].clear();
				auto& state = EcologyPythonManager::states[i].back();
				auto& reward = EcologyPythonManager::rewards[i].back();
				pybind11::object result = EcologyPythonManager::pyModule.attr("Update")(state, reward);
				EcologyPythonManager::actions[i].push_back(result.cast<Ebony::Box>());
				if (!Ebony::SystemManager::shouldResetForAi)
				{
					EcologyPythonManager::states[i].clear();
					EcologyPythonManager::rewards[i].clear();
				}
			}
		}
	}

	void EcologyPythonManager::Reset()
	{
		for (auto i = 0; i < EcologyPythonManager::states.size(); i++)
		{
			if (EcologyPythonManager::states[i].size() != 0)
			{
				EcologyPythonManager::actions[i].clear();
				auto& state = EcologyPythonManager::states[i].back();
				auto& reward = EcologyPythonManager::rewards[i].back();
				EcologyPythonManager::pyModule.attr("Reset")(state, reward);
				EcologyPythonManager::states[i].clear();
				EcologyPythonManager::rewards[i].clear();
			}
		}
	}


	void EcologyPythonManager::ProcessInput()
	{
	}
	void EcologyPythonManager::Init(const char* filename, std::uint64_t environmentCount)
	{
		pybind11::initialize_interpreter();



		EcologyPythonManager::pyModule = pybind11::module_::import(filename);
		//EcologyPythonManager::stateModule = pybind11::module_::import("cpp_module");
		EcologyPythonManager::initialized = true;

		EcologyPythonManager::states.resize(environmentCount);
		EcologyPythonManager::actions.resize(environmentCount);
		EcologyPythonManager::rewards.resize(environmentCount);



		EcologyPythonManager::pyModule.attr("Start")();
	}


	void EcologyPythonManager::Destroy()
	{
		if (EcologyPythonManager::initialized)
		{
			EcologyPythonManager::pyModule.release();
			pybind11::finalize_interpreter();
		}
	}
}