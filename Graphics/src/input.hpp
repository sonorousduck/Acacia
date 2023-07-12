#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "camera.hpp"
#include <unordered_map>
#include <functional>

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