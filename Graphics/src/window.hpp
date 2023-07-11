#pragma once

#include "graphics2d.hpp"
#include "input.hpp"


namespace Ebony 
{
	class Window
	{
	public:
		static GLFWwindow* createWindow(Graphics2d* graphics);
	};

}