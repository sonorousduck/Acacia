#include "input.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"


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

	void KeyInput::saveKeyBindings(std::string_view filepath, components::KeyboardInput& keyboardInput)
	{
		rapidjson::Document document{};
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		document.SetObject();

		/*for (auto iter = input->bindings.begin(); iter != input->bindings.end(); iter++)
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
		}*/


		// Iterate through the bindings first
		// iter->first = Key
		// iter->second = functionString
		// Json Structure:
		// bindings: {
		//	key: string,
		//	key: string,
		// },


		// TODO: THIS MAY NEED TO BE ADDING MEMBERS TO THE OBJECT AND NOT THE VALUE

		rapidjson::Value bindingNestedObject(rapidjson::kObjectType);
		document.AddMember("bindings", bindingNestedObject, allocator);
		rapidjson::Value& bindingNestedObjectValue = document["bindings"];
		
		for (auto iter = keyboardInput.bindings.begin(); iter != keyboardInput.bindings.end(); iter++)
		{
			char* keyString = (char*)(iter->first);
			bindingNestedObjectValue.AddMember(rapidjson::GenericStringRef(keyString), iter->second, allocator);
		}

		rapidjson::Value onPressNestedObject(rapidjson::kObjectType);
		document.AddMember("onPress", onPressNestedObject, allocator);
		rapidjson::Value& onPressNestedObjectValue = document["onPress"];
		// Iterate through the onPressActions
		for (auto iter = keyboardInput.onPressActions.begin(); iter != keyboardInput.onPressActions.end(); iter++)
		{
			onPressNestedObjectValue.AddMember(rapidjson::StringRef((char*)(iter->first.data())), iter->second, allocator);
		}

		// Iterate through the onReleaseActions
		rapidjson::Value onReleaseNestedObject(rapidjson::kObjectType);
		document.AddMember("onRelease", onReleaseNestedObject, allocator);
		rapidjson::Value& onReleaseNestedObjectValue = document["onPress"];

		for (auto iter = keyboardInput.onReleaseActions.begin(); iter != keyboardInput.onReleaseActions.end(); iter++)
		{
			onReleaseNestedObjectValue.AddMember(rapidjson::StringRef((char*)(iter->first.data())), iter->second, allocator);
		}

		// Iterate through the onHeldActions
		rapidjson::Value onHeldNestedObject(rapidjson::kObjectType);
		document.AddMember("onHeld", onReleaseNestedObject, allocator);
		rapidjson::Value& onHeldNestedObjectValue = document["onHeld"];

		for (auto iter = keyboardInput.onHeldActions.begin(); iter != keyboardInput.onHeldActions.end(); iter++)
		{
			onHeldNestedObjectValue.AddMember(rapidjson::StringRef((char*)(iter->first.data())), iter->second, allocator);
		}

	}

	void KeyInput::loadKeyBindings(std::string_view filepath, components::KeyboardInput& keyboardInput)
	{

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

				if (result == PressedState::RELEASED)
				{
					m_buttons[button] = PressedState::NONE;
				}
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

	PressedState ControllerInput::setIsTriggerDown(int button, float value)
	{
		std::unordered_map<int, PressedState>::iterator it = m_joystick_triggers.find(button);

		if (it != m_joystick_triggers.end())
		{
			PressedState state = m_joystick_triggers[button];
			PressedState finalState = PressedState::NONE;

			if (abs(value) > 0.10f) 
			{
				finalState = PressedState::PRESSED;
			}
			else if (state == PressedState::PRESSED)
			{
				finalState = PressedState::RELEASED;
			}
			else if (state == PressedState::RELEASED)
			{
				finalState = PressedState::NONE;
			}

			m_joystick_triggers[button] = finalState;
		}
		return m_joystick_triggers[button];
	}
}
