#pragma once

#include "system.hpp"
#include "../components/animationControllerComponent.hpp"
#include "animationRenderer.hpp"

#include <chrono>

namespace systems
{
	class Animation2d : public System
	{
	public:
		Animation2d() : System({ ctti::unnamed_type_id<components::AnimationController>() })
		{
		}

		virtual void Update(std::chrono::microseconds elapsedTime) override;
		friend Animation2d;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
	};


}
