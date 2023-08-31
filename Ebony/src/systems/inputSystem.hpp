#pragma once

#include "system.hpp"

#include "../components/inputComponent.hpp"
#include "../components/mouseInputComponent.hpp"

#include "input.hpp"
#include <optional>

namespace systems
{
	class InputSystem : public System
	{
	public:
		InputSystem() : System({ ctti::unnamed_type_id<components::Input>(), ctti::unnamed_type_id<components::MouseInput>()})
		{
		}

		void Update(Ebony::KeyInput& keyInput, Ebony::MouseInput& mouseInput);
		
	protected:
		virtual bool isInterested(const entities::EntityPtr& entity);

	private:
		using System::Update;
		void updateImpl(Ebony::KeyInput& keyInput, Ebony::MouseInput& mouseInput);
	};
}