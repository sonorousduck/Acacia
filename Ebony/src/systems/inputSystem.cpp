#include "inputSystem.hpp"

#include <algorithm>
#include "../singletons/inputManager.hpp"

namespace systems
{
	void InputSystem::Update()
	{
		this->updateImpl();
	}


	// Every input component has actions for both the keyboard and the controller
	// though they may not actually be used or registered

	//Ebony::KeyInput& keyInput, Ebony::MouseInput& mouseInput, Ebony::ControllerInput& controllerInput

	void InputSystem::updateImpl()
	{
		for (auto& [id, entity] : m_Entities)
		{
			// Handle Keyboard first
			components::KeyboardInput* keyInputComponent{};
			if (entity->tryGetComponent<components::KeyboardInput>(keyInputComponent))
			{
				// We have all our bindings, so we need to determine which state each is in
				for (auto iter = keyInputComponent->bindings.begin(); iter != keyInputComponent->bindings.end(); iter++)
				{
					Ebony::PressedState pressedState = Ebony::InputManager::keyboardInstance->getKeyState(iter->first);
					
					if (pressedState & Ebony::PressedState::PRESSED && keyInputComponent->onPressActions.contains(iter->second))
					{
						keyInputComponent->onPressActions[iter->second](entity);
					}
					if (pressedState & Ebony::PressedState::HELD && keyInputComponent->onHeldActions.contains(iter->second))
					{
						keyInputComponent->onHeldActions[iter->second](entity);
					}
					else if (pressedState & Ebony::PressedState::RELEASED && keyInputComponent->onReleaseActions.contains(iter->second))
					{
						keyInputComponent->onReleaseActions[iter->second](entity);
					}
				}
			}



			// Handle Controller Input
			components::ControllerInput* controllerInputComponent{};
			if (entity->tryGetComponent(controllerInputComponent))
			{

				if (Ebony::InputManager::controllersConnected > controllerInputComponent->joystickId)
				{
					std::shared_ptr controllerInstance = Ebony::InputManager::controllerInstances[Ebony::InputManager::sdlJoystickToJoystickConversion[controllerInputComponent->joystickId]];

					// Look for button inputs
					for (auto iter = controllerInputComponent->bindings.begin(); iter != controllerInputComponent->bindings.end(); iter++)
					{
						Ebony::PressedState pressedState = controllerInstance->getButtonState(iter->first);

						if (pressedState & Ebony::PressedState::PRESSED && controllerInputComponent->onPressActions.contains(iter->second))
						{
							controllerInputComponent->onPressActions[iter->second](entity);
						}
						else if (pressedState & Ebony::PressedState::HELD && controllerInputComponent->onHeldActions.contains(iter->second))
						{
							controllerInputComponent->onHeldActions[iter->second](entity);
						}
						else if (pressedState & Ebony::PressedState::RELEASED && controllerInputComponent->onReleaseActions.contains(iter->second))
						{
							controllerInputComponent->onReleaseActions[iter->second](entity);
						}
					}


					// Look for Joystick/Trigger inputs
					for (auto iter = controllerInputComponent->joystickBindings.begin(); iter != controllerInputComponent->joystickBindings.end(); iter++)
					{
						Ebony::JoystickAndTriggerPress pressedState = controllerInstance->getTriggerJoystickState(iter->first);

						if ((pressedState.pressedState.current & (Ebony::PressedState::PRESSED | Ebony::PressedState::HELD)) && controllerInputComponent->joystickActions.contains(iter->second))
						{
							controllerInputComponent->joystickActions[iter->second](entity, pressedState.scaledCurrentValue);
						}
					}
				}
			}
			
			// Handle Mouse input
			if (entity->hasComponent<components::MouseInput>())
			{
				auto mouseInputComponent = entity->getComponent<components::MouseInput>();

				for (auto iter = mouseInputComponent->bindings.begin(); iter != mouseInputComponent->bindings.end(); iter++)
				{
					Ebony::MousePress mousePress = Ebony::InputManager::mouseInstance->getMouseState(iter->first);

					if (mousePress.pressInfo.current & Ebony::PressedState::PRESSED && mouseInputComponent->onPressActions.contains(iter->second))
					{
						mouseInputComponent->onPressActions[iter->second](entity, mousePress);
					}
					else if (mousePress.pressInfo.current & Ebony::PressedState::HELD && mouseInputComponent->onHeldActions.contains(iter->second))
					{
						mouseInputComponent->onHeldActions[iter->second](entity, mousePress);
					}
					else if (mousePress.pressInfo.current & Ebony::PressedState::RELEASED && mouseInputComponent->onReleaseActions.contains(iter->second))
					{
						mouseInputComponent->onReleaseActions[iter->second](entity, mousePress);
					}
				}
			}
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


		return doICare;
	}



}