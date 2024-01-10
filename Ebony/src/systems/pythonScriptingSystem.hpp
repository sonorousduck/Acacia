#pragma once


#include "system.hpp"
#include "../components/pythonScriptComponent.hpp"
#include <pybind11/pybind11.h>

namespace systems
{
	class PythonScriptingSystem : public System
	{
	public:
		PythonScriptingSystem() : System({ ctti::unnamed_type_id<components::PythonScript>() })
		{

		}

		void Start();

		void Update(std::chrono::microseconds elapsedTime) override;


	protected:


	private:
		void updateImpl(std::chrono::microseconds elapsedTime);

		bool hasStarted = false;
	};
}