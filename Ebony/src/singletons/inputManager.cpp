#include "inputManager.hpp"
#include <graphics2d.hpp>

namespace Ebony
{
	std::vector<KeyInputManager*> InputManager::keyboardInstances;
	std::unordered_map<SDL_JoystickID, std::shared_ptr<ControllerInputManager>> InputManager::controllerInstances;
	std::vector<MouseInputManager*> InputManager::mouseInstances;
	std::vector<AIInputManager*> InputManager::aiInstances;
	std::unordered_map<SDL_JoystickID, SDL_GameController*> InputManager::controllers;

	KeyInputManager::KeyInputManager() : isEnabled(true)
	{
		InputManager::keyboardInstances.push_back(this);
	}

	KeyInputManager::~KeyInputManager()
	{
		InputManager::keyboardInstances.erase(std::remove(InputManager::keyboardInstances.begin(), InputManager::keyboardInstances.end(), this), InputManager::keyboardInstances.end());
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
					//EB_TRACE("IF I WROTE THIS RIGHT, I SHOULDN'T SEE THIS! (inputManager.cpp)");
					result.current = HELD;
				}
				else if (result.current & RELEASED && result.previous & RELEASED)
				{
					//EB_TRACE("IF I WROTE THIS RIGHT, I SHOULDN'T SEE THIS! (inputManager.cpp)");
					result.current = NONE;
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
			else if (pressedState & PRESSED && result.current & HELD)
			{
				result.previous = HELD;
			}
			else if (pressedState & RELEASED && result.current & RELEASED)
			{
				result.current = NONE;
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
			case SDL_KEYUP:

				EB_TRACE(SDL_GetKeyName(event.key.keysym.sym));
				break;

			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERAXISMOTION:
				EB_TRACE(event.type);
				break;
				
			case SDL_CONTROLLERDEVICEADDED:
				InputManager::controllers[event.cdevice.which] = SDL_GameControllerOpen(event.cdevice.which);
				InputManager::controllerInstances[event.cdevice.which] = std::move(std::make_shared<ControllerInputManager>(event.cdevice.which));
				EB_TRACE("ADDING NEW CONTROLLER (inputManager.cpp)");
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				InputManager::controllers.erase(event.cdevice.which);
				InputManager::controllerInstances.erase(event.cdevice.which);
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
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			if (SDL_IsGameController(i))
			{

				InputManager::controllers[i] = SDL_GameControllerOpen(i);
				InputManager::controllerInstances[i] = std::move(std::make_shared<ControllerInputManager>(i));

				
			}
		}
	}

	ControllerInputManager::ControllerInputManager(int joystickId) : joystickId(joystickId)
	{
	}


	ControllerInputManager::~ControllerInputManager()
	{
	}

}