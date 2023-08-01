#pragma once

#include "system.hpp"
#include "../components/animationControllerComponent.hpp"
#include <graphics2d.hpp>

namespace systems
{
	class AnimationRenderer : public System
	{

	public:
		AnimationRenderer() : System({ ctti::unnamed_type_id<components::AnimationController>() }) {};

		void Update(Ebony::Graphics2d& graphics);

	private:
		using System::Update; // disables compiler warning from clang


	};

}