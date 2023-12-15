#pragma once

#include "system.hpp"
#include "../components/sprite.hpp"
#include <graphics2d.hpp>
#include "../components/transform.hpp"

namespace systems
{
	class SpriteRenderer : public System
	{
	public:
		SpriteRenderer() : System({ ctti::unnamed_type_id<components::Sprite>(), ctti::unnamed_type_id<components::Transform>() })
		{
		}


		void Update();
		bool debug = true;

	private:
		using System::Update;
	};
}