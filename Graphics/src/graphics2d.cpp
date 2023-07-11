#include "graphics2d.hpp"
#include "window.hpp"


// https://www.glfw.org/docs/3.3/input_guide.html
namespace Ebony
{
	Graphics2d::Graphics2d()
	{
	}

	void Graphics2d::Initialize(const char* windowName, int width, int height) {
		windowName = windowName;
		screenWidth = width;
		screenHeight = height;
		versionMajor = 3;
		versionMinor = 3;

		input = new Input();

		Window glfwWindow{};
		window = glfwWindow.createWindow(this);
		input->setupJoystickInputs();
		Initialize();
	}

	void Graphics2d::onCursorPos(double xPosIn, double yPosIn)
	{
		this->input->onCursorPos(xPosIn, yPosIn);
	}

	void Graphics2d::onFramebufferSizeChange(int width, int height)
	{
		glViewport(0, 0, width, height);
		screenWidth = width;
		screenHeight = height;
	}

	void Graphics2d::onScroll(double xOffset, double yOffset)
	{
		this->input->onScroll(xOffset, yOffset);
	}

	void Graphics2d::onMouseButton(int button, int action, int mods)
	{
		this->input->onMouseButton(button, action, mods);
	}

	void Graphics2d::onKeyInput(int button, int scancode, int action, int mods)
	{
		this->input->onKeyInput(button, scancode, action, mods);
	}



	//void Graphics2d::onInput(int input)
	//{
		/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}*/

		/*if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}*/
	//}



	void Graphics2d::Initialize()
	{
		lastMosX = screenWidth / 2.0f;
		lastMosY = screenHeight / 2.0f;


		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
	}




}
