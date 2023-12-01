#pragma once

#include "system.hpp"
#include "../components/text.hpp"
#include <graphics2d.hpp>
#include "../components/transform.hpp"


namespace systems
{
	class FontRenderer : public System
	{
	public:
		FontRenderer() : System({ ctti::unnamed_type_id<components::Text>(), ctti::unnamed_type_id<components::Transform>()}) {};

		void Update();

	private:
		using System::Update;
	};
}