#pragma once

#include "system.hpp"
#include "../components/sprite.hpp"
#include <graphics2d.hpp>
#include "../components/transform.hpp"
#include "../components/collider.hpp"
#include "../components/colliderSprite.hpp"

namespace systems
{
	class ColliderRenderer : public System
	{
	public:
		ColliderRenderer() : System({ ctti::unnamed_type_id<components::Collider>(), ctti::unnamed_type_id<components::Transform>(), ctti::unnamed_type_id<components::ColliderSprite>()})
		{
		}


		void Update(Ebony::Graphics2d& graphics);

	private:
		using System::Update;
	};
}