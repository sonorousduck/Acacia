#include "inputSystem.hpp"

#include <algorithm>
#include <iostream>

namespace systems
{
	void InputSystem::Update(Ebony::KeyInput& keyInput, Ebony::MouseInput& mouseInput, Ebony::ControllerInput& controllerInput)
	{
		this->updateImpl(keyInput, mouseInput, controllerInput);
	}


	// Every input component has actions for both the keyboard and the controller
	// though they may not actually be used or registered
	void InputSystem::updateImpl(Ebony::KeyInput& keyInput, Ebony::MouseInput& mouseInput, Ebony::ControllerInput& controllerInput)
	{
		for (auto& [id, entity] : m_Entities)
		{

			// Handle Keyboard first
			if (entity->hasComponent<components::KeyboardInput>())
			{
				auto input = entity->getComponent<components::KeyboardInput>();

				// We have all our bindings, so we need to determine which state each is in

				for (auto iter = input->bindings.begin(); iter != input->bindings.end(); iter++)
				{
					Ebony::PressedState pressedState = keyInput.getIsKeyDown(iter->first);

					if (pressedState == Ebony::PressedState::PRESSED && input->onPressActions.contains(iter->second))
					{
						input->onPressActions[iter->second]();
					}
					else if (pressedState == Ebony::PressedState::HELD && input->onHeldActions.contains(iter->second))
					{
						input->onHeldActions[iter->second]();
					}
					else if (pressedState == Ebony::PressedState::RELEASED && input->onReleaseActions.contains(iter->second))
					{
						input->onReleaseActions[iter->second]();
					}
				}
			}

			// Handle Controller Input
			if (entity->hasComponent<components::ControllerInput>()) 
			{
				auto input = entity->getComponent<components::ControllerInput>();

				if (Ebony::KeyInput::joysticksConnected > 0)
				{
					GLFWgamepadstate state{};
					if (glfwGetGamepadState(input->joystickId, &state))
					{
						for (auto iter = input->bindings.begin(); iter != input->bindings.end(); iter++)
						{
							// Update bindings information and get it as well
							Ebony::PressedState pressedState = controllerInput.setIsButtonDown(iter->first, state.buttons[iter->first]);

							if (pressedState == Ebony::PressedState::PRESSED && input->onPressActions.contains(iter->second))
							{
								input->onPressActions[iter->second]();
							}
							else if (pressedState == Ebony::PressedState::HELD && input->onHeldActions.contains(iter->second))
							{
								input->onHeldActions[iter->second]();
							}
							else if (pressedState == Ebony::PressedState::RELEASED && input->onReleaseActions.contains(iter->second))
							{
								input->onReleaseActions[iter->second]();
							}
						}

						for (auto iter = input->joystickBindings.begin(); iter != input->joystickBindings.end(); iter++)
						{
							// Update the joystick (and trigger) information
							Ebony::PressedState pressedState = controllerInput.setIsTriggerDown(iter->first, state.axes[iter->first]);

							if ((pressedState == Ebony::PressedState::PRESSED || pressedState == Ebony::PressedState::RELEASED) && input->onPressActions.contains(iter->second))
							{
								input->joystickActions[iter->second](state.axes[iter->first]);
							}
						}



						//for (auto iter = input->controllerActionKeyPairs.begin(); iter != input->controllerActionKeyPairs.end(); iter++)
						//{
						//if (state.buttons[iter->first] && input->previousActions[iter->first] != GLFW_PRESS)
						//	{
						//		iter->second();
						//	}
						//	// TODO: Potentially, I will want this to have onPress, onRelease, and onHold callbacks. But for now, suffices to just have on press
						//	input->previousActions[iter->first] = state.buttons[iter->first];

						//	// There are also state.axes which are the joystick positions. This will need to be translated as well, but might have its own callbacks since you need to take in a float
						//	// The axis indices are GLFW_GAMEPAD_AXIS_LEFT_X, GLFW_GAMEPAD_AXIS_LEFT_Y, GLFW_GAMEPAD_AXIS_RIGHT_X, GLFW_GAMEPAD_AXIS_RIGHT_Y, GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
						//}

						//for (auto iter = input->controllerAxes.begin(); iter != input->controllerAxes.end(); iter++)
						//{
						//	if (state.axes[iter->first])
						//	{
						//		iter->second(state.axes[iter->first]);
						//	}
						//}
					}
				}
			}
			
			// Handle Mouse input
			if (entity->hasComponent<components::MouseInput>())
			{
				auto input = entity->getComponent<components::MouseInput>();

				for (auto iter = input->bindings.begin(); iter != input->bindings.end(); iter++)
				{
					Ebony::PressedState pressedState = mouseInput.getIsButtonDown(iter->first);

					if (pressedState == Ebony::PressedState::PRESSED && input->onPressActions.contains(iter->second))
					{
						input->onPressActions[iter->second]();
					}
					else if (pressedState == Ebony::PressedState::HELD && input->onHeldActions.contains(iter->second))
					{
						input->onHeldActions[iter->second]();
					}
					else if (pressedState == Ebony::PressedState::RELEASED && input->onReleaseActions.contains(iter->second))
					{
						input->onReleaseActions[iter->second]();
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

	bool InputSystem::isInterested(const entities::EntityPtr& entity)
	{
		bool doICare = std::any_of(
			m_Interests.begin(), m_Interests.end(),
			[&entity](auto interest)
			{
				return entity->getComponents().contains(interest);
			});
		if (doICare)
		{
			std::cout << "YEP!" << std::endl;
		}

		return doICare;
	}



}