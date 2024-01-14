#pragma once

#include "../entity.hpp"
#include "../screens/screen.hpp"

#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

namespace Ebony
{
	class PythonManager
	{

	public:

        static void Update(std::chrono::microseconds elapsedTime);
        static void ProcessInput();

		static void Init(const char* filename);
		static void Destroy();

        // Vector of environments of actions
        static std::vector<std::vector<std::string>> actions;

	private:
		PythonManager() {}


		static bool initialized;
		static pybind11::module pyModule;




	};
}