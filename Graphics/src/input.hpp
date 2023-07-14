#pragma once

#include <vector>
#include <iostream>
#include "camera.hpp"
#include <unordered_map>
#include <functional>
#include "window.hpp"


namespace Ebony
{
	enum class PressedState
	{
		NONE,
		PRESSED,
		HELD,
		RELEASED
	};

	struct Press
	{
		PressedState current;
		PressedState previous;
	};

	// Credit to https://stackoverflow.com/questions/55573238/how-do-i-do-a-proper-input-class-in-glfw-for-a-game-engine
	class KeyInput
	{
	public:
		KeyInput() { m_isEnabled = true;};
		KeyInput(std::vector<int> keysToMonitor);
		~KeyInput();


		// If KeyInput is enabled (i.e. you can disable UI keyboard in gameplay) and the key is pressed down, return the pressed state
		PressedState getIsKeyDown(int key);
		bool getKeyPressedOrHeld(int key);
		bool getIsEnabled() { return m_isEnabled; }
		void setIsEnabled(bool value) { m_isEnabled = value; };

		void setKeysToMonitorInit(std::vector<int> keysToMonitor);

	private:
		void setIsKeyDown(int key, PressedState isDown);

		std::unordered_map<int, PressedState> m_keys;
		bool m_isEnabled;


	public:
		static void setupKeyInputs(Window& window);

	private:
		static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static std::vector<KeyInput*> _instances;
	};









	class Input
	{
	public:
		Input();


		void onScroll(double xOffset, double yOffset);
		void onMouseButton(int button, int action, int mods);
		void onKeyInput(int key, int scancode, int action, int mods);
		void onCursorPos(double x, double y);

		void setupJoystickInputs();
		void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime);

		//void AddOnPressCallback(int key, std::function<void()> func);
		//void AddOnReleaseCallback(int key, std::function<void()> func);
		//void AddOnHeldCallback(int key, std::function<void()> func);


		float lastMosX = 0.0f;
		float lastMosY = 0.0f;
		bool firstMouse = true;
		bool cursorDisabled = true;


		unsigned int numControllersConnected = 0;
		unsigned int maxNumControllersConnected = 4; // Actual limit is 16
		std::vector<GLFWgamepadstate> gamepadStates;


		std::unordered_map<int, Press> keyPressed;
		std::unordered_map<int, bool> pressedKeys;
		std::unordered_map<int, bool> releasedKeys;
		std::unordered_map<int, bool> heldKeys;
	};
}