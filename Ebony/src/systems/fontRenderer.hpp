#pragma once

#include "system.hpp"
#include "../components/text.hpp"
#include <graphics2d.hpp>


namespace systems
{
	class FontRenderer : public System
	{
	public:
		FontRenderer() : System({ ctti::unnamed_type_id<components::Text>() }) {};

		void Update(Ebony::Graphics2d& graphics);

	private:
		using System::Update;
	};
}