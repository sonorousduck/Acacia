#include "SpaceGuyPythonManager.hpp"


namespace SpaceGuy
{
	std::vector<std::vector<Ebony::Box>> SpaceGuyPythonManager::actions{};
	std::vector<std::vector<State>> SpaceGuyPythonManager::states{};
	std::vector<std::vector<Ebony::Discrete>> SpaceGuyPythonManager::rewards{};

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



	void SpaceGuyPythonManager::Update(std::chrono::microseconds elapsedTime)
	{
		for (auto i = 0; i < SpaceGuyPythonManager::states.size(); i++)
		{
			if (SpaceGuyPythonManager::states[i].size() != 0)
			{
				SpaceGuyPythonManager::actions[i].clear();
				auto& state = SpaceGuyPythonManager::states[i].back();
				auto& reward = SpaceGuyPythonManager::rewards[i].back();
				pybind11::object result = SpaceGuyPythonManager::pyModule.attr("Update")(state, reward);
				SpaceGuyPythonManager::actions[i].push_back(result.cast<Ebony::Box>());
				if (!Ebony::SystemManager::shouldResetForAi)
				{
					SpaceGuyPythonManager::states[i].clear();
					SpaceGuyPythonManager::rewards[i].clear();
				}
			}
		}
	}

	void SpaceGuyPythonManager::Reset()
	{
		for (auto i = 0; i < SpaceGuyPythonManager::states.size(); i++)
		{
			if (SpaceGuyPythonManager::states[i].size() != 0)
			{
				SpaceGuyPythonManager::actions[i].clear();
				auto& state = SpaceGuyPythonManager::states[i].back();
				auto& reward = SpaceGuyPythonManager::rewards[i].back();
				SpaceGuyPythonManager::pyModule.attr("Reset")(state, reward);
				SpaceGuyPythonManager::states[i].clear();
				SpaceGuyPythonManager::rewards[i].clear();
			}
		}
	}


	void SpaceGuyPythonManager::ProcessInput()
	{
	}
	void SpaceGuyPythonManager::Init(const char* filename, std::uint64_t environmentCount)
	{
		pybind11::initialize_interpreter();



		SpaceGuyPythonManager::pyModule = pybind11::module_::import(filename);
		//SpaceGuyPythonManager::stateModule = pybind11::module_::import("cpp_module");
		SpaceGuyPythonManager::initialized = true;

		SpaceGuyPythonManager::states.resize(environmentCount);
		SpaceGuyPythonManager::actions.resize(environmentCount);
		SpaceGuyPythonManager::rewards.resize(environmentCount);



		SpaceGuyPythonManager::pyModule.attr("Start")();
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