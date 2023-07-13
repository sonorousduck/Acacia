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



		auto mouse_callback = [](GLFWwindow* window, double xPosIn, double yPosIn)
		{
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onCursorPos(xPosIn, yPosIn);
		};

		auto scroll_callback = [](GLFWwindow* window, double x, double y)
		{
			static_cast<Input*>(glfwGetWindowUserPointer(window))->onScroll(x, y);
		};

		auto framebuffer_callback = [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onFramebufferSizeChange(width, height);
		};

		auto mouse_button_callback = [](GLFWwindow* window, int button, int action, int mods)
		{
			static_cast<Input*>(glfwGetWindowUserPointer(window))->onMouseButton(button, action, mods);
		};

		auto key_callback = [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onKeyInput(key, scancode, action, mods);
		};


		glfwSetWindowUserPointer(window, graphics);
		glfwSetFramebufferSizeCallback(window, framebuffer_callback);

		//glfwSetWindowUserPointer(window, graphics->input);

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetKeyCallback(window, key_callback);


		glfwMakeContextCurrent(window);

		return window;
	}
}
