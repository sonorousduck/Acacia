#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "../Log.hpp"

#include "../Core.hpp"
#include <SDL.h>
#include <glm/glm.hpp>

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

	struct JoystickAndTriggerPress
	{
		short currentValue;
		short lastValue;
		float scaledCurrentValue;
		float scaledLastValue;

		Press pressedState;
	};

	struct MouseEvent
	{
		Uint8 button;
		Uint16 clicks;
		int x;
		int y;
	};


	struct MousePress
	{
		MouseEvent mouseClick;
		MouseEvent mouseRelease;
		
		Press pressInfo;
	};

	struct MouseScroll
	{
		int xPos;
		int yPos;
		int mouseScrollX;
		int mouseScrollY;
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

		PressedState getButtonState(Uint8 button);
		JoystickAndTriggerPress getTriggerJoystickState(Uint8 trigger);

		// Takes in left and right from 0.0 - 1.0, it manually converts it to the maximums for you
		void Vibrate(float left, float right, Uint32 ms, bool vibrateTriggers);
		
		void setIsButtonDown(Uint8 button, PressedState pressedState);
		void setIsTriggerDown(Uint8 button, short value);

		int joystickId;
		bool isXboxController = false;

	private:
		std::unordered_map<Uint8, Press> buttons;
		std::unordered_map<Uint8, JoystickAndTriggerPress> joysticksAndTriggers;

	};

	class MouseInputManager
	{
	public:
		MouseInputManager();
		~MouseInputManager();

		PressedState getMouseState();
		void setMouseState();
		void setMousePosition(int x, int y, int xRel, int yRel);

		int getMouseRelativeX();
		int getMouseRelativeY();
		glm::vec2 getMouseRelative();

		int getMouseAbsoluteX();
		int getMouseAbsoluteY();
		glm::vec2 getMouseAbsolute();

		void resetScroll();
		void setMouseScroll(int x, int y, int mouseScrollX, int mouseScrollY);
		MouseScroll getMouseScroll();

	private:
		std::unordered_map<Uint8, MousePress> buttons;

		int absoluteX = 0;
		int absoluteY = 0;

		int relativeX = 0;
		int relativeY = 0;

		MouseScroll mouseScroll;
	};

	class AIInputManager
	{

	};

	class InputManager
	{
	public:
		bool static HandleInput();
		void static Initialize();
		void static Vibrate(std::uint8_t joystickId, float left, float right, Uint32 ms, bool vibrateTriggers);
		void static ResetInput();


		static std::unordered_map<SDL_JoystickID, SDL_GameController*> controllers;
		static std::unordered_map<SDL_JoystickID, std::shared_ptr<ControllerInputManager>> controllerInstances;
		static std::unordered_map<SDL_JoystickID, std::uint8_t> sdlJoystickToJoystickConversion;
		static std::shared_ptr<KeyInputManager> keyboardInstance;
		static std::uint8_t controllersConnected;
		static std::shared_ptr<MouseInputManager> mouseInstance;
		static std::vector<std::shared_ptr<AIInputManager>> aiInstances;
	};
}
