#include "CryptPythonManager.hpp"


namespace Crypt
{
	std::vector<std::vector<Ebony::Box>> CryptPythonManager::actions{};
	std::vector<std::vector<State>> CryptPythonManager::states{};
	std::vector<std::vector<Ebony::Discrete>> CryptPythonManager::rewards{};

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



	void CryptPythonManager::Update(std::chrono::microseconds elapsedTime)
	{
		for (auto i = 0; i < CryptPythonManager::states.size(); i++)
		{
			if (CryptPythonManager::states[i].size() != 0)
			{
				CryptPythonManager::actions[i].clear();
				auto& state = CryptPythonManager::states[i].back();
				auto& reward = CryptPythonManager::rewards[i].back();
				pybind11::object result = CryptPythonManager::pyModule.attr("Update")(state, reward);
				CryptPythonManager::actions[i].push_back(result.cast<Ebony::Box>());
				if (!Ebony::SystemManager::shouldResetForAi)
				{
					CryptPythonManager::states[i].clear();
					CryptPythonManager::rewards[i].clear();
				}
			}
		}
	}

	void CryptPythonManager::Reset()
	{
		for (auto i = 0; i < CryptPythonManager::states.size(); i++)
		{
			if (CryptPythonManager::states[i].size() != 0)
			{
				CryptPythonManager::actions[i].clear();
				auto& state = CryptPythonManager::states[i].back();
				auto& reward = CryptPythonManager::rewards[i].back();
				CryptPythonManager::pyModule.attr("Reset")(state, reward);
				CryptPythonManager::states[i].clear();
				CryptPythonManager::rewards[i].clear();
			}
		}
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
		CryptPythonManager::rewards.resize(environmentCount);



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