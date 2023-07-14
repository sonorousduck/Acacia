#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Ebony 
{
	class Window
	{
	public:
		Window() {};
		void createWindow(int versionMajor, int versionMinor);
		void createWindow(const char* windowName, int versionMajor, int versionMinor);

		GLFWwindow* getWindow();

	private:
		GLFWwindow* window{};
	};

}