#pragma once

#include "system.hpp"

#include "../components/inputComponent.hpp"
#include "input.hpp"

namespace systems
{
	class InputSystem : public System
	{
	public:
		InputSystem() : System({ ctti::unnamed_type_id<components::Input>() })
		{
		}

		void Update(Ebony::KeyInput& keyInput) ;

	private:
		using System::Update;
		void updateImpl(Ebony::KeyInput& keyInput);
	};
}