#include "window.hpp"
#include "graphics2d.hpp"

namespace Ebony
{
	void Window::createWindow(int versionMajor, int versionMinor)
	{
		if (!glfwInit())
		{
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		window = glfwCreateWindow(800, 600, "TEST", NULL, NULL);

		if (!window)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}


		glfwSetWindowUserPointer(window, this);
		glfwMakeContextCurrent(window);

		return;
	}

	void Window::createWindow(const char* windowName, int versionMajor, int versionMinor)
	{
		if (!glfwInit())
		{
			return;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		window = glfwCreateWindow(800, 600, windowName, NULL, NULL);

		if (!window)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}


		glfwSetWindowUserPointer(window, this);
		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);
		return;
	}

	GLFWwindow* Window::getWindow()
	{
		return window;
	}

}
