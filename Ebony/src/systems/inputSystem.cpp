#include "inputSystem.hpp"

#include <iostream>

namespace systems
{
	void InputSystem::Update(Ebony::KeyInput& keyInput)
	{
		this->updateImpl(keyInput);
	}


	// Every input component has actions for both the keyboard and the controller
	// though they may not actually be used or registered
	void InputSystem::updateImpl(Ebony::KeyInput& keyInput)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto input = entity->getComponent<components::Input>();

			for (auto iter = input->keyboardActionKeyPairs.begin(); iter != input->keyboardActionKeyPairs.end(); iter++)
			{
				Ebony::PressedState pressedState = keyInput.getIsKeyDown(iter->first);
				
				if (pressedState == Ebony::PressedState::PRESSED)
				{
					iter->second();
				}
			}

			if (Ebony::KeyInput::joysticksConnected > 0)
			{
				for (auto iter = input->controllerActionKeyPairs.begin(); iter != input->controllerActionKeyPairs.end(); iter++)
				{
					// Unfortunately, there isn't a nice way to have a callback system like key input, for some reason. But we can manually check for all controller buttons here

					GLFWgamepadstate state{};

					if (glfwGetGamepadState(input->joystickId, &state))
					{
						if (state.buttons[iter->first])
						{
							iter->second();
						}

						// There are also state.axes which are the joystick positions. This will need to be translated as well, but might have its own callbacks since you need to take in a float
						// The axis indices are GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_GAMEPAD_AXIS_LEFT_Y, GLFW_GAMEPAD_AXIS_RIGHT_X, GLFW_GAMEPAD_AXIS_RIGHT_Y, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER

					}

				}
			}

			
			/*if (entity->hasComponent<components::KeyboardInput>())
			{
				std::cout << "Found keyboard input" << std::endl;
			}

			if (entity->hasComponent<components::ControllerComponent>())
			{
				std::cout << "Found controller input" << std::endl;

			}*/


		}
	}



}