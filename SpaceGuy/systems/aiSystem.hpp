#pragma once

#include <systems/system.hpp>
#include <components/aiComponent.hpp>
#include <components/transform.hpp>
#include "../misc/aiInformationTypes.hpp"
#include <graphics2d.hpp>
#include "../singletons/SpaceGuyPythonManager.hpp"
#include <algorithm>

namespace systems
{
	class AISystem : public System
	{
	public:
		AISystem() : System({ ctti::unnamed_type_id<components::AIComponent>() })
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};
}