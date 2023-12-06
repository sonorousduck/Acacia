//#pragma once
//
//#include <vector>
//#include <iostream>
//#include "camera.hpp"
//#include <unordered_map>
//#include <functional>
//#include "window.hpp"
//#include "GLFW/glfw3.h"
//
//namespace Ebony
//{
//	/*enum PressedState : std::uint8_t
//	{
//		NONE = 0,
//		PRESSED = 1,
//		HELD = 2,
//		RELEASED = 4
//	};
//
//	struct Press
//	{
//		PressedState current;
//		PressedState previous;
//	};*/
//
//	// Credit to https://stackoverflow.com/questions/55573238/how-do-i-do-a-proper-input-class-in-glfw-for-a-game-engine
//	//class KeyInput
//	//{
//	//public:
//	//	KeyInput() { m_isEnabled = true;};
//	//	KeyInput(std::vector<int> keysToMonitor);
//	//	~KeyInput();
//
//
//	//	// If KeyInput is enabled (i.e. you can disable UI keyboard in gameplay) and the key is pressed down, return the pressed state
//	//	PressedState getIsKeyDown(int key);
//	//	bool getKeyPressedOrHeld(int key);
//	//	bool getIsEnabled() { return m_isEnabled; }
//	//	void setIsEnabled(bool value) { m_isEnabled = value; };
//
//	//	void setKeysToMonitorInit(std::vector<int> keysToMonitor);
//
//	//	
//
//	//private:
//	//	void setIsKeyDown(int key, PressedState isDown);
//
//	//	std::unordered_map<int, PressedState> m_keys;
//	//	std::unordered_map<int, PressedState> m_lastFrame;
//	//	bool m_isEnabled;
//
//
//	//public:
//	//	static void setupKeyInputs(Window& window);
//	//	static int joysticksConnected;
//
//	//private:
//	//	static void callback(GLFWwindow* window, int key, int scancode, int action, int mods);
//	//	static void joystick_callback(int jid, int event);
//
//	//	static std::vector<KeyInput*> _instances;
//	//};
//
//	//class MouseInput
//	//{
//	//public:
//	//	MouseInput() { m_isEnabled = true; }
//	//	MouseInput(std::vector<int> buttonsToMonitor);
//	//	~MouseInput();
//
//	//	static void setupMouseInputs(Window& window);
//	//	void setButtonsToMonitor(std::vector<int> buttonsToMonitor);
//
//	//	bool getButtonPressedOrHeld(int button);
//	//	PressedState getIsButtonDown(int button);
//	//	double xPos = 0.0, yPos = 0.0;
//
//
//	//private:
//	//	void setIsButtonDown(int key, PressedState isDown);
//
//	//	std::unordered_map<int, PressedState> m_buttons;
//	//	bool m_isEnabled;
//
//
//	//	static void mouse_position_callback(GLFWwindow* window, double xPos, double yPos);
//	//	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
//
//	//	static std::vector<MouseInput*> _instances;
//	//};
//
//	//class ControllerInput
//	//{
//	//public:
//	//	ControllerInput() { m_isEnabled = true; };
//	//	ControllerInput(std::vector<int> buttonsToMonitor);
//	//	~ControllerInput();
//
//	//	PressedState getIsButtonDown(int button);
//	//	bool getIsEnabled() { return m_isEnabled; }
//	//	void setIsEnabled(bool value) { m_isEnabled = value; }
//
//	//	void setButtonsToMonitorInit(std::vector<int> buttonsToMonitor);
//
//	//	// Must be public due to Controllers not having callback functions available
//	//	PressedState setIsButtonDown(int button, unsigned int isDown);
//	//	PressedState setIsTriggerDown(int button, float value);
//
//	//private:
//
//
//	//	std::unordered_map<int, PressedState> m_buttons;
//	//	std::unordered_map<int, PressedState> m_joystick_triggers;
//	//	bool m_isEnabled;
//	//	static std::vector<ControllerInput*> _instances;
//
//	//};
//
//	//class Input
//	//{
//	//public:
//	//	Input();
//
//
//	//	void onScroll(double xOffset, double yOffset);
//	//	void onMouseButton(int button, int action, int mods);
//	//	void onKeyInput(int key, int scancode, int action, int mods);
//	//	void onCursorPos(double x, double y);
//
//	//	void setupJoystickInputs();
//	//	void ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime);
//
//	//	//void AddOnPressCallback(int key, std::function<void()> func);
//	//	//void AddOnReleaseCallback(int key, std::function<void()> func);
//	//	//void AddOnHeldCallback(int key, std::function<void()> func);
//
//
//	//	float lastMosX = 0.0f;
//	//	float lastMosY = 0.0f;
//	//	bool firstMouse = true;
//	//	bool cursorDisabled = true;
//
//
//	//	unsigned int numControllersConnected = 0;
//	//	unsigned int maxNumControllersConnected = 4; // Actual limit is 16
//	//	std::vector<GLFWgamepadstate> gamepadStates;
//
//
//	//	std::unordered_map<int, Press> keyPressed;
//	//	std::unordered_map<int, bool> pressedKeys;
//	//	std::unordered_map<int, bool> releasedKeys;
//	//	std::unordered_map<int, bool> heldKeys;
//	//};
//}