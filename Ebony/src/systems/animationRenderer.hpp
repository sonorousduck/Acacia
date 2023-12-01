#pragma once

#include "system.hpp"
#include "../components/animationControllerComponent.hpp"
#include <graphics2d.hpp>
#include "../components/transform.hpp"

namespace systems
{
	class AnimationRenderer : public System
	{

	public:
		AnimationRenderer() : System({ ctti::unnamed_type_id<components::AnimationController>(), ctti::unnamed_type_id<components::Transform>() }) {};

		void Update();

	private:
		using System::Update; // disables compiler warning from clang


	};

}