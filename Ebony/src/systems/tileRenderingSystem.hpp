#pragma once

#include "system.hpp"
#include "../components/sprite.hpp"
#include <graphics2d.hpp>
#include "../components/transform.hpp"
#include "../components/tileComponent.hpp"

namespace systems
{
	class TileRenderer : public System
	{
	public:
		TileRenderer() : System({ ctti::unnamed_type_id<components::Sprite>(), ctti::unnamed_type_id<components::Transform>(), ctti::unnamed_type_id<components::Tile>() })
		{
		}


		void Update();
		bool debug = true;

	private:
		using System::Update;
	};
}