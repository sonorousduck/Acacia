#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../Log.hpp"

#include "../Core.hpp"
#include <SDL.h>

namespace Ebony
{
	enum PressedState : std::uint8_t
	{
		NONE = BIT(0),
		PRESSED = BIT(1),
		HELD = BIT(2),
		REPEAT = BIT(3),
		RELEASED = BIT(4)
	};

	struct Press
	{
		PressedState current = NONE;
		PressedState previous = NONE;
	};

	class KeyInputManager
	{
	public:
		KeyInputManager();
		~KeyInputManager();

		PressedState getKeyState(SDL_Keycode key);
		void setIsKeyDown(SDL_Keycode key, PressedState pressedState);



	private:
		bool isEnabled;
		std::unordered_map<SDL_Keycode, Press> keys;


	};

	class ControllerInputManager
	{
	public:
		ControllerInputManager(int joystickId);
		~ControllerInputManager();


		int joystickId;

	};

	class MouseInputManager
	{

	};

	class AIInputManager
	{

	};

	class InputManager
	{
	public:
		bool static HandleInput();
		void static Initialize();


		static std::unordered_map<SDL_JoystickID, SDL_GameController*> controllers;
		static std::unordered_map<SDL_JoystickID, std::shared_ptr<ControllerInputManager>> controllerInstances;
		static std::shared_ptr<KeyInputManager> keyboardInstance;
		static std::shared_ptr<MouseInputManager> mouseInstance;
		static std::vector<std::shared_ptr<AIInputManager>> aiInstances;
	};
}
