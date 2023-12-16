#pragma once

#include "system.hpp"

#include "../components/cppScriptComponent.hpp"


namespace systems
{
	class CppScriptingSystem : public System
	{
	public:
		CppScriptingSystem() : System({ ctti::unnamed_type_id<components::CppScript>() })
		{}

		void Start();

		void Update(std::chrono::microseconds elapsedTime) override;


	protected:


	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}