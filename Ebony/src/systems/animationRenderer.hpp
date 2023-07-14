#pragma once

#include "system.hpp"
#include "animation2d.hpp"
#include <graphics2d.hpp>

namespace systems
{
	class AnimationRenderer : public System
	{

	public:
		void Update(systems::Animation2d& animationSystem, Ebony::Graphics2d& graphics);

	private:
		using System::Update; // disables compiler warning from clang


	};

}