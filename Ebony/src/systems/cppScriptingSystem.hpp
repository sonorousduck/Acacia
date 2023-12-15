#pragma once

#include "system.hpp"
#include "../components/cppScriptComponent.hpp"

namespace systems
{
	class CppScriptingSystem : public System
	{
	public:
		CppScriptingSystem() : System({ ctti::unnamed_type_id<components::CppScriptBase>() })
		{

		}

		void Update(std::chrono::microseconds elapsedTime) override;


	protected:
		void updateImpl(std::chrono::microseconds elapsedTime);
		
		bool isInterested(const entities::EntityPtr& entity);

	};
}