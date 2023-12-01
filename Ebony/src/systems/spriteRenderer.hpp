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


		void Update(std::shared_ptr<Ebony::Graphics2d>);

	private:
		using System::Update;
		bool debug = true;
	};
}