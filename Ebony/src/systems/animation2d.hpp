#pragma once

#include "system.hpp"
#include "../animationControllerComponent.hpp"

#include <chrono>

namespace systems
{
	class Animation2d : public System
	{
		Animation2d() : System({ ctti::unnamed_type_id<components::AnimationController>() })
		{
		}

		virtual void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};


}
