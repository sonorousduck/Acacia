#include "window.hpp"

namespace Ebony
{
	GLFWwindow* Window::createWindow(Graphics2d* graphics)
	{
		GLFWwindow* window{};
		if (!glfwInit())
		{
			return nullptr;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, graphics->versionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, graphics->versionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		window = glfwCreateWindow(800, 600, "TEST", NULL, NULL);

		if (!window)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return nullptr;
		}


		glfwSetWindowUserPointer(window, graphics);

		auto mouse_callback = [](GLFWwindow* window, double xPosIn, double yPosIn)
		{
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onCursorPos(xPosIn, yPosIn);
		};

		auto scroll_callback = [](GLFWwindow* window, double x, double y)
		{
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onScroll(x, y);
		};

		auto framebuffer_callback = [](GLFWwindow* window, int width, int height)
		{
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onFramebufferSizeChange(width, height);
		};

		auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onMouseButton(button, action, mods);
		};

		auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onKeyInput(key, scancode, action, mods);
		};

		 //This has to move probably since it can't call the window
		auto joystick_connection = [](int jid, int event)
		{
			if (event == GLFW_CONNECTED)
			{
				std::cout << "Connected!" << std::endl;
			}
			else if (event == GLFW_DISCONNECTED)
			{
				std::cout << "Disconnected!" << std::endl;
			}
		};

		if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
		{
			// Use as gamepad
			std::cout << "This is a gamepad" << std::endl;
			const char* name = glfwGetGamepadName(GLFW_JOYSTICK_1);
			std::cout << "It has the name: " << name << std::endl;
		}

		// Joysticks sadly operate slightly differently, as they don't take window* as a pointer. This means that I will need to set up the callbacks inside of graphics2d instead... Though in 
		// all reality, I feel like all of this should be in a different class from graphics. It probably should be in Input



		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetFramebufferSizeCallback(window, framebuffer_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetKeyCallback(window, key_callback);
		glfwSetJoystickCallback(joystick_connection);


		glfwMakeContextCurrent(window);

		return window;
	}
}
