#include "inputManager.hpp"
#include <graphics2d.hpp>

namespace Ebony
{
	std::shared_ptr<KeyInputManager> InputManager::keyboardInstance{};
	std::unordered_map<SDL_JoystickID, std::shared_ptr<ControllerInputManager>> InputManager::controllerInstances{};
	std::unordered_map<SDL_JoystickID, std::uint8_t> InputManager::sdlJoystickToJoystickConversion{};
	std::uint8_t InputManager::controllersConnected{0};
	std::shared_ptr<MouseInputManager> InputManager::mouseInstance{};
	std::vector<std::shared_ptr<AIInputManager>> InputManager::aiInstances{};
	std::unordered_map<SDL_JoystickID, SDL_GameController*> InputManager::controllers{};

	KeyInputManager::KeyInputManager() : isEnabled(true)
	{
	}

	KeyInputManager::~KeyInputManager()
	{
	}

	PressedState KeyInputManager::getKeyState(SDL_Keycode key)
	{
		Press result{};

		if (isEnabled)
		{
			auto it = keys.find(key);

			if (it != keys.end())
			{
				result = keys[key];

				if (result.current & PRESSED && result.previous & PRESSED)
				{
					result.current = HELD;
				}
				else if (result.current & RELEASED && result.previous & RELEASED)
				{
					result.current = NONE;
				}
				else
				{
					result.previous = result.current;
				}

				keys[key] = result;
			}
			else
			{
				keys[key] = Press();
			}
		}
		return result.current;
	}

	void KeyInputManager::setIsKeyDown(SDL_Keycode key, PressedState pressedState)
	{
		auto it = keys.find(key);

		if (it != keys.end())
		{
			Press result = keys[key];

			result.previous = result.current;

			if (pressedState & PRESSED && result.current & PRESSED)
			{
				result.current = HELD;
			}
			else if (pressedState & RELEASED && result.current & RELEASED)
			{
				result.current = NONE;
			}
			else
			{
				result.current = pressedState;
			}

			keys[key] = result;
		}
		else
		{
			auto pressed = Press();
			pressed.current = pressedState;
			keys[key] = pressed;
		}

	}

	bool InputManager::HandleInput()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				return true;
				break;

			case SDL_KEYDOWN:
				InputManager::keyboardInstance->setIsKeyDown(event.key.keysym.sym, PRESSED);
				EB_TRACE(event.key.keysym.sym);
				break;

			case SDL_KEYUP:
				InputManager::keyboardInstance->setIsKeyDown(event.key.keysym.sym, RELEASED);
				break;

			case SDL_CONTROLLERBUTTONDOWN:
				InputManager::controllerInstances[InputManager::sdlJoystickToJoystickConversion[event.cdevice.which]]->setIsButtonDown(event.cbutton.button, PRESSED);
				break;

			case SDL_CONTROLLERBUTTONUP:
				InputManager::controllerInstances[InputManager::sdlJoystickToJoystickConversion[event.cdevice.which]]->setIsButtonDown(event.cbutton.button, RELEASED);
				break;

			case SDL_CONTROLLERAXISMOTION:

				// event.caxis.value ranges from (-32768 to 32767)
				switch (event.caxis.axis)
				{
				// Left X axis moved (Left is negative, right positive)
				case 0:
					EB_INFO(event.caxis.value);
					break;

				// Left Y axis moved (Down is positive, up is negative)
				case 1:
					break;

				// Right X axis moved (Left is negative, right positive)
				case 2:
					break;

				// Right Y axis moved (Down is positive, up is negative)
				case 3:
					break;

				// Trigger Left axis moved
				case 4:
					break;

				// Trigger Right axis moved
				case 5:
					break;
				
				// Controller Axis Max
				case 6:
					EB_TRACE("WHAT DID I JUST PRESS? I DON'T KNOW WHAT SDL_CONTROLLER_AXIS_MAX IS");
				}
				break;
				
			case SDL_CONTROLLERDEVICEADDED:
				InputManager::controllers[event.cdevice.which] = SDL_GameControllerOpen(event.cdevice.which);
				InputManager::controllerInstances[event.cdevice.which] = std::move(std::make_shared<ControllerInputManager>(event.cdevice.which));
				InputManager::sdlJoystickToJoystickConversion[SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(InputManager::controllers[event.cdevice.which]))] = event.cdevice.which;
				InputManager::controllersConnected++;
				EB_TRACE("ADDING NEW CONTROLLER (inputManager.cpp)");
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				InputManager::controllerInstances.erase(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(InputManager::controllers[event.cdevice.which])));
				InputManager::controllers.erase(event.cdevice.which);
				InputManager::sdlJoystickToJoystickConversion.erase(SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(InputManager::controllers[event.cdevice.which])));

				InputManager::controllersConnected--;
				EB_TRACE("REMOVING CONTROLLER (inputManager.cpp)");
				break;

			case SDL_CONTROLLERTOUCHPADMOTION:
				EB_TRACE(event.type);
				EB_TRACE("RUMBLE RUMBLE!");

				if (event.ctouchpad.x < 0.5)
				{
					SDL_JoystickRumble(SDL_GameControllerGetJoystick(InputManager::controllers[0]), 65535, 0, 100);
				}
				else
				{
					SDL_JoystickRumble(SDL_GameControllerGetJoystick(InputManager::controllers[0]), 0, 65535, 100);
				}
				break;

			case SDL_JOYBATTERYUPDATED:
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					// data1 is width
					// data2 is height	


					Graphics2d::setNewWindowSize(event.window.data1, event.window.data2);
				}
				break;

			}
		}

		return false;
	}

	void InputManager::Initialize()
	{
		InputManager::controllers.reserve(SDL_NumHaptics());
		InputManager::controllerInstances.reserve(SDL_NumHaptics());

		/*for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i))
			{
				InputManager::controllers[i] = SDL_GameControllerOpen(i);
				InputManager::controllerInstances[i] = std::move(std::make_shared<ControllerInputManager>(i));
				controllersConnected++;
			}
		}*/

		InputManager::keyboardInstance = std::make_shared<KeyInputManager>();
		InputManager::mouseInstance = std::make_shared<MouseInputManager>();

	}

	ControllerInputManager::ControllerInputManager(int joystickId) : joystickId(joystickId)
	{
	}


	ControllerInputManager::~ControllerInputManager()
	{
	}

	void ControllerInputManager::setIsButtonDown(Uint8 button, PressedState pressedState)
	{
		auto it = buttons.find(button);

		if (it != buttons.end())
		{
			Press result = buttons[button];

			result.previous = result.current;

			if (pressedState & PRESSED && result.current & PRESSED)
			{
				result.current = HELD;
			}
			else if (pressedState & RELEASED && result.current & RELEASED)
			{
				result.current = NONE;
			}
			else
			{
				result.current = pressedState;
			}

			buttons[button] = result;
		}
		else
		{
			auto pressed = Press();
			pressed.current = pressedState;
			buttons[button] = pressed;
		}
	}

	PressedState ControllerInputManager::getTriggerJoystickState(Uint8 trigger)
	{
		return PressedState();
	}


	PressedState ControllerInputManager::getButtonState(Uint8 button)
	{
		Press result{};

		auto it = buttons.find(button);

		if (it != buttons.end())
		{
			result = buttons[button];

			if (result.current & PRESSED && result.previous & PRESSED)
			{
				result.current = HELD;
			}
			else if (result.current & RELEASED && result.previous & RELEASED)
			{
				result.current = NONE;
			}
			else
			{
				result.previous = result.current;
			}

			buttons[button] = result;
		}
		else
		{
			buttons[button] = Press();
			
		}
		return result.current;
	}

	void ControllerInputManager::setIsTriggerDown(Uint8 button, float value)
	{
	}




}