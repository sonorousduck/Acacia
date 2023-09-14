#include "input.hpp"

namespace Ebony
{

	std::vector<KeyInput*> KeyInput::_instances;
	std::vector<MouseInput*> MouseInput::_instances;

	int KeyInput::joysticksConnected = 0;

	KeyInput::KeyInput(std::vector<int> keysToMonitor) : m_isEnabled(true)
	{
		for (int i = 0; i < keysToMonitor.size(); i++)
		{
			m_keys[keysToMonitor[i]] = PressedState::NONE;
		}

		KeyInput::_instances.push_back(this);
	}

	KeyInput::~KeyInput()
	{
		_instances.erase(std::remove(_instances.begin(), _instances.end(), this), _instances.end());
	}

	void KeyInput::setKeysToMonitorInit(std::vector<int> keysToMonitor)
	{
		for (int i = 0; i < keysToMonitor.size(); i++)
		{
			m_keys[keysToMonitor[i]] = PressedState::NONE;
		}
		KeyInput::_instances.push_back(this);
	}

	PressedState KeyInput::getIsKeyDown(int key)
	{
		PressedState result = PressedState::NONE;

		if (m_isEnabled)
		{
			std::unordered_map<int, PressedState>::iterator it = m_keys.find(key);

			if (it != m_keys.end())
			{
				result = m_keys[key];

				if (result == PressedState::RELEASED)
				{
					m_keys[key] = PressedState::NONE;
				}
			}
		}
		return result;
	}

	bool KeyInput::getKeyPressedOrHeld(int key)
	{

		if (m_isEnabled)
		{
			std::unordered_map<int, PressedState>::iterator it = m_keys.find(key);

			if (it != m_keys.end())
			{
				PressedState state = m_keys[key];

				if (state == PressedState::PRESSED || state == PressedState::HELD)
				{
					return true;
				}
			}
		}
		return false;
	}

	void KeyInput::setIsKeyDown(int key, PressedState isDown)
	{
		std::unordered_map<int, PressedState>::iterator it = m_keys.find(key);
		if (it != m_keys.end())
		{
			PressedState state = m_keys[key];
			
			if (isDown == PressedState::PRESSED && state == PressedState::PRESSED)
			{
				isDown = PressedState::HELD;
			}
			else if (isDown == PressedState::PRESSED && state == PressedState::HELD)
			{
				isDown = PressedState::HELD;
			}
			else if (isDown == PressedState::RELEASED && state == PressedState::RELEASED)
			{
				isDown = PressedState::NONE;
			}
			
			m_keys[key] = isDown;
		}
	}

	void KeyInput::setupKeyInputs(Window& window)
	{
		glfwSetKeyCallback(window.getWindow(), KeyInput::callback);
		glfwSetJoystickCallback(KeyInput::joystick_callback);
		
		int i = 0;
		while (glfwJoystickPresent(i))
		{
			joysticksConnected++;
			i++;
		}
	}

	void KeyInput::callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		for (KeyInput* keyInput : _instances)
		{
			PressedState state = PressedState::NONE;
			if (action == GLFW_PRESS)
			{
				state = PressedState::PRESSED;
			}
			else if (action == GLFW_REPEAT)
			{
				state = PressedState::HELD;
			}
			else if (action == GLFW_RELEASE)
			{
				state = PressedState::RELEASED;
			}

			keyInput->setIsKeyDown(key, state);
		}
	}

	void KeyInput::joystick_callback(int jid, int event)
	{
		if (event == GLFW_CONNECTED)
		{
			std::cout << "Joystick connected" << std::endl;
			KeyInput::joysticksConnected++;
		}
		else if (event == GLFW_DISCONNECTED)
		{
			std::cout << "Joystick disconnected" << std::endl;
			KeyInput::joysticksConnected--;
		}
	}

	MouseInput::MouseInput(std::vector<int> buttonsToMonitor) : m_isEnabled(true)
	{
		for (int i = 0; i < buttonsToMonitor.size(); i++)
		{
			m_buttons[buttonsToMonitor[i]] = PressedState::NONE;
		}

		MouseInput::_instances.push_back(this);
	}

	MouseInput::~MouseInput()
	{
		_instances.erase(std::remove(_instances.begin(), _instances.end(), this), _instances.end());
	}

	void MouseInput::setButtonsToMonitor(std::vector<int> buttonsToMonitor)
	{
		for (int i = 0; i < buttonsToMonitor.size(); i++)
		{
			m_buttons[buttonsToMonitor[i]] = PressedState::NONE;
		}

		MouseInput::_instances.push_back(this);
	}

	PressedState MouseInput::getIsButtonDown(int button)
	{
		PressedState result = PressedState::NONE;

		if (m_isEnabled)
		{
			std::unordered_map<int, PressedState>::iterator it = m_buttons.find(button);

			if (it != m_buttons.end())
			{
				result = m_buttons[button];
			}
		}

		return result;
	}

	bool MouseInput::getButtonPressedOrHeld(int button)
	{
		if (m_isEnabled)
		{
			std::unordered_map<int, PressedState>::iterator it = m_buttons.find(button);

			if (it != m_buttons.end())
			{
				PressedState state = m_buttons[button];

				return (state == PressedState::PRESSED || state == PressedState::HELD);
			}
		}
		return false;
	}

	void MouseInput::mouse_position_callback(GLFWwindow* window, double xPos, double yPos)
	{
		//glfwGetCursorPos(window, &xPos, &yPos);
		for (MouseInput* mouseInput : _instances)
		{
			mouseInput->xPos = xPos;
			mouseInput->yPos = yPos;
		}
	}

	void MouseInput::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		for (MouseInput* mouseInput : _instances)
		{
			PressedState state = PressedState::NONE;
			if (action == GLFW_PRESS)
			{
				state = PressedState::PRESSED;
			}
			else if (action == GLFW_REPEAT)
			{
				state = PressedState::HELD;
			}
			else if (action == GLFW_RELEASE)
			{
				state = PressedState::RELEASED;
			}

			mouseInput->setIsButtonDown(button, state);

		}
	}

	void MouseInput::setIsButtonDown(int button, PressedState isDown)
	{
		std::unordered_map<int, PressedState>::iterator it = m_buttons.find(button);
		if (it != m_buttons.end())
		{
			PressedState state = m_buttons[button];
			
			if (isDown == PressedState::PRESSED && state == PressedState::PRESSED)
			{
				isDown = PressedState::HELD;
			}
			else if (isDown == PressedState::PRESSED && state == PressedState::HELD)
			{
				isDown = PressedState::HELD;
			}
			else if (isDown == PressedState::RELEASED && state == PressedState::RELEASED)
			{
				isDown = PressedState::NONE;
			}

			m_buttons[button] = isDown;
		}
	}

	void MouseInput::setupMouseInputs(Window& window)
	{
		glfwSetCursorPosCallback(window.getWindow(), mouse_position_callback);
		glfwSetMouseButtonCallback(window.getWindow(), mouse_button_callback);
	}

		//	glfwSetKeyCallback(window.getWindow(), KeyInput::callback);
		//	glfwSetJoystickCallback(KeyInput::joystick_callback);

		//	int i = 0;
		//	while (glfwJoystickPresent(i))
		//	{
		//		joysticksConnected++;
		//		i++;
		//	
		//}
	




	//Input::Input()
	//{
	//}

	//void Input::onCursorPos(double xPosIn, double yPosIn)
	//{
	//	float x = static_cast<float>(xPosIn);
	//	float y = static_cast<float>(yPosIn);

	//	if (firstMouse)
	//	{
	//		lastMosX = x;
	//		lastMosY = y;
	//		firstMouse = false;
	//	}


	//	float xOffset = x - lastMosX;
	//	float yOffset = lastMosY - y;
	//	lastMosX = x;
	//	lastMosY = y;
	//}

	//void Input::onMouseButton(int button, int action, int mods)
	//{
	//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	//	{
	//		std::cout << "Pressed" << std::endl;
	//	}
	//	std::cout << button << ", " << action << ", " << mods << std::endl;
	//}

	//// Good to keep, so you don't have to check all the buttons and which ones are pressed. You can just immediately add them to the map
	//void Input::onKeyInput(int button, int scancode, int action, int mods)
	//{
	//	if (button == GLFW_KEY_E && action == GLFW_PRESS)
	//	{
	//		std::cout << "E PRESSED!" << std::endl;
	//	}

	//	keyPressed[button].previous = keyPressed[button].current;
	//	if (action == GLFW_PRESS)
	//	{
	//		keyPressed[button].current = PressedState::PRESSED;
	//	}
	//	else if (action == GLFW_RELEASE)
	//	{
	//		keyPressed[button].current = PressedState::NONE;
	//	}
	//}

	//void Input::onScroll(double xOffset, double yOffset)
	//{
	//	std::cout << "Scrolling" << std::endl;
	//}

	//void Input::setupJoystickInputs()
	//{
	//	gamepadStates.reserve(maxNumControllersConnected);

	//	for (unsigned int i = 0; i < maxNumControllersConnected; i++)
	//	{
	//		gamepadStates.push_back(GLFWgamepadstate());
	//	}
	//}

	//// This will eventually not take the camera or deltaTime. It should really just create a map that other things can read from to see what keys are pressed
	//void Input::ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime)
	//{
	//	
	//	for (auto i = keyPressed.begin(); i != keyPressed.end(); i++)
	//	{

	//		if (i->second.current != i->second.previous && i->second.previous == PressedState::NONE)
	//		{
	//			// Calling onPress callback
	//			std::cout << "Pressed " << i->first << std::endl;
	//			i->second.previous = i->second.current;
	//			pressedKeys[i->first] = true;
	//		}
	//		else if (i->second.current == PressedState::NONE && i->second.previous == PressedState::HELD)
	//		{
	//			// Calling onRelease
	//			std::cout << "Released " << i->first << std::endl;
	//			i->second.previous = PressedState::NONE;
	//			i->second.current = PressedState::NONE;
	//			releasedKeys[i->first] = true;

	//		}
	//		else if (i->second.current == PressedState::PRESSED && i->second.previous == PressedState::PRESSED)
	//		{
	//			// Calling onHeld
	//			std::cout << "Held " << i->first << std::endl;
	//			i->second.current = PressedState::HELD;
	//			i->second.previous = PressedState::HELD;
	//			heldKeys[i->first] = true;
	//		}
	//	}


	//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	{
	//		glfwSetWindowShouldClose(window, true);
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	//	{
	//		camera.ProcessKeyboard(FORWARD, deltaTime);
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	//	{
	//		camera.ProcessKeyboard(BACKWARD, deltaTime);
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	//	{
	//		camera.ProcessKeyboard(LEFT, deltaTime);
	//	}
	//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	//	{
	//		camera.ProcessKeyboard(RIGHT, deltaTime);
	//	}

	//	for (int i = 0; i < maxNumControllersConnected; i++)
	//	{
	//		if (glfwGetGamepadState(i, &gamepadStates[i]))
	//		{
	//			if (numControllersConnected < i + 1)
	//			{
	//				std::cout << "Connected" << std::endl;
	//				numControllersConnected++;
	//			}


	//			if (gamepadStates[i].buttons[GLFW_GAMEPAD_BUTTON_A])
	//			{
	//				std::cout << "X pressed" << std::endl;
	//			}

	//		}
	//		else
	//		{
	//			if (numControllersConnected > i)
	//			{
	//				std::cout << "Disconnected" << std::endl;
	//				numControllersConnected--;
	//			}

	//		}
	//	}
	//}
}
