#include "graphics2d.hpp"
#include "window.hpp"
#include "resourceManager.hpp"


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


		//input = Input();
		window = Window();
		window.createWindow(versionMajor, versionMinor);
		//input.setupJoystickInputs();
		Initialize();
		SetupCallback();

		projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
		initRenderData();
	}

	void Graphics2d::Initialize(const char* windowName, int width, int height, int majorVersion, int minorVersion)
	{
		windowName = windowName;
		screenWidth = width;
		screenHeight = height;
		versionMajor = majorVersion;
		versionMinor = minorVersion;

		//input = Input();

		window = Window();
		window.createWindow(versionMajor, versionMinor);
		SetupCallback();
		//input.setupJoystickInputs();
		Initialize();
	}

	void Graphics2d::SetupCallback()
	{
		auto framebuffer_callback = [](GLFWwindow* window, int width, int height)
		{
			std::cout << "Called" << std::endl;
			glViewport(0, 0, width, height);
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onFramebufferSizeChange(width, height);
		};
		
		glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_callback);
	}

	/*void Graphics2d::onCursorPos(double xPosIn, double yPosIn)
	{
		this->input.onCursorPos(xPosIn, yPosIn);
	}*/

	void Graphics2d::onFramebufferSizeChange(int width, int height)
	{
		std::cout << "HERE TOO!" << std::endl;
		screenWidth = width;
		screenHeight = height;
	}

	/*void Graphics2d::onScroll(double xOffset, double yOffset)
	{
		std::cout << "WENT THROUGH THIS" << std::endl;
		this->input.onScroll(xOffset, yOffset);
	}

	void Graphics2d::onMouseButton(int button, int action, int mods)
	{
		this->input.onMouseButton(button, action, mods);
	}

	void Graphics2d::onKeyInput(int button, int scancode, int action, int mods)
	{
		this->input.onKeyInput(button, scancode, action, mods);
	}*/

	void Graphics2d::Initialize()
	{
		lastMosX = screenWidth / 2.0f;
		lastMosY = screenHeight / 2.0f;


		glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwMakeContextCurrent(window.getWindow());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
	}

	void Graphics2d::initRenderData()
	{
		// Since all sprites share the same vertex data, you only need one of these
		unsigned int VBO;
		float vertices[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &this->quadVAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(this->quadVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	void Graphics2d::BeginDraw(Color clearColor)
	{
		glClearColor(clearColor.r(), clearColor.g(), clearColor.b(), clearColor.a());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Graphics2d::EndDraw()
	{
		glfwSwapBuffers(window.getWindow());
	}


	void Graphics2d::Draw(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color)
	{
		// This one will use a default shader that will already be loaded into graphics
		Shader& s = ResourceManager::GetShader("default");

		s.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // then rotate
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

		if (hasCamera)
		{
			s.setMat4("view", mainCamera.GetViewMatrix());
		}
		else
		{
			s.setMat4("view", glm::mat4(1.0f));
		}

		s.setMat4("model", model);
		s.setVec3("spriteColor", Colors::White.GetRGB());

		glActiveTexture(GL_TEXTURE0);
		texture.Bind();

		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Graphics2d::Draw(Shader& s, Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color)
	{
		/*s.use();
		glm::mat4 projection = glm::perspective(glm::radians(mainCamera.Zoom), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);
		glm::mat4 view = mainCamera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		s.setMat4("projection", projection);
		s.setMat4("view", view);*/


	}

	

	void Graphics2d::Cleanup()
	{
		glDeleteVertexArrays(1, &quadVAO);
		//glDeleteFramebuffers(1, &quadVBO);

	}

	void Graphics2d::SetMainCamera(Camera& camera)
	{
		mainCamera = camera;
		hasCamera = true;
	}

}
