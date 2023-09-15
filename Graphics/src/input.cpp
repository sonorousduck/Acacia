#include "input.hpp"

namespace Ebony
{

	std::vector<KeyInput*> KeyInput::_instances;
	std::vector<MouseInput*> MouseInput::_instances;
	std::vector<ControllerInput*> ControllerInput::_instances;

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
		std::cout << "TODO: Move this from KeyInput to ControllerInput and auto set things up when a controller is connected" << std::endl;
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

	ControllerInput::ControllerInput(std::vector<int> buttonsToMonitor) : m_isEnabled(true)
	{
		for (int i = 0; i < buttonsToMonitor.size(); i++)
		{
			m_buttons[buttonsToMonitor[i]] = PressedState::NONE;
		}

		ControllerInput::_instances.push_back(this);
	}
	
	ControllerInput::~ControllerInput()
	{
		_instances.erase(std::remove(_instances.begin(), _instances.end(), this), _instances.end());
	}

	void ControllerInput::setButtonsToMonitorInit(std::vector<int> buttonsToMonitor)
	{
		for (int i = 0; i < buttonsToMonitor.size(); i++)
		{
			m_buttons[buttonsToMonitor[i]] = PressedState::NONE;
		}
		ControllerInput::_instances.push_back(this);
	}

	PressedState ControllerInput::getIsButtonDown(int button)
	{
		PressedState result = PressedState::NONE;

		if (m_isEnabled)
		{
			std::unordered_map<int, PressedState>::iterator it = m_buttons.find(button);

			if (it != m_buttons.end())
			{
				result = m_buttons[button];

				if (result == PressedState::RELEASED)
				{
					m_buttons[button] = PressedState::NONE;
				}
			}
		}
		return result;
	}

	PressedState ControllerInput::setIsButtonDown(int button, unsigned int isDown)
	{
		std::unordered_map<int, PressedState>::iterator it = m_buttons.find(button);
		if (it != m_buttons.end())
		{
			PressedState state = m_buttons[button];
			PressedState finalState = PressedState::NONE;

			if (isDown == GLFW_RELEASE && (state == PressedState::PRESSED || state == PressedState::HELD))
			{
				finalState = PressedState::RELEASED;
			}
			else if (isDown == GLFW_REPEAT)
			{
				finalState = PressedState::HELD;
			}
			else if (isDown == GLFW_PRESS && state == PressedState::PRESSED)
			{
				finalState = PressedState::HELD;
			}
			else if (isDown == GLFW_PRESS)
			{
				finalState = PressedState::PRESSED;
			}
			else if (isDown == GLFW_RELEASE && (state == PressedState::RELEASED || state == PressedState::NONE))
			{
				finalState = PressedState::NONE;
			}

			m_buttons[button] = finalState;
		}

		// Return this, since you will want it immediately anyways
		return m_buttons[button];
	}


}
