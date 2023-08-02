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

			for (auto iter = input->onReleaseKeyboardActionKeyPairs.begin(); iter != input->onReleaseKeyboardActionKeyPairs.end(); iter++)
			{
				Ebony::PressedState pressedState = keyInput.getIsKeyDown(iter->first);

				if (pressedState == Ebony::PressedState::RELEASED)
				{
					iter->second();
				}
			}

			if (Ebony::KeyInput::joysticksConnected > 0)
			{
				GLFWgamepadstate state{};
				if (glfwGetGamepadState(input->joystickId, &state))
				{

					for (auto iter = input->controllerActionKeyPairs.begin(); iter != input->controllerActionKeyPairs.end(); iter++)
					{
						if (state.buttons[iter->first] && input->previousActions[iter->first] != GLFW_PRESS)
						{
							iter->second();
						}
						// TODO: Potentially, I will want this to have onPress, onRelease, and onHold callbacks. But for now, suffices to just have on press
						input->previousActions[iter->first] = state.buttons[iter->first];

						// There are also state.axes which are the joystick positions. This will need to be translated as well, but might have its own callbacks since you need to take in a float
						// The axis indices are GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_GAMEPAD_AXIS_LEFT_Y, GLFW_GAMEPAD_AXIS_RIGHT_X, GLFW_GAMEPAD_AXIS_RIGHT_Y, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
					}

					for (auto iter = input->controllerAxes.begin(); iter != input->controllerAxes.end(); iter++)
					{
						if (state.axes[iter->first])
						{
							iter->second(state.axes[iter->first]);
						}
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