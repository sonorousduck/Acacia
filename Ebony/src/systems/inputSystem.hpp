#pragma once

#include <optional>

#include "system.hpp"
#include "input.hpp"

#include "../components/mouseInputComponent.hpp"
#include "../components/controllerComponent.hpp"
#include "../components/keyboardComponent.hpp"


namespace systems
{
	class InputSystem : public System
	{
	public:
		InputSystem() : System({ ctti::unnamed_type_id<components::KeyboardInput>(), ctti::unnamed_type_id<components::ControllerInput>(), ctti::unnamed_type_id<components::MouseInput>()})
		{
		}

		void Update(Ebony::KeyInput& keyInput, Ebony::MouseInput& mouseInput, Ebony::ControllerInput& controllerInput);
		
	protected:
		virtual bool isInterested(const entities::EntityPtr& entity);

	private:
		using System::Update;
		void updateImpl(Ebony::KeyInput& keyInput, Ebony::MouseInput& mouseInput, Ebony::ControllerInput& controllerInput);
	};
}