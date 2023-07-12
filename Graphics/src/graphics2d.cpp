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


		input = Input();

		Window glfwWindow{};
		window = glfwWindow.createWindow(this);
		input.setupJoystickInputs();
		Initialize();

		projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
		initRenderData();
		defaultShader.LoadShader("shaders/sprite.vert", "shaders/sprite.frag");

	}

	void Graphics2d::Initialize(const char* windowName, int width, int height, int majorVersion, int minorVersion)
	{
		windowName = windowName;
		screenWidth = width;
		screenHeight = height;
		versionMajor = majorVersion;
		versionMinor = minorVersion;

		input = Input();

		Window glfwWindow{};
		window = glfwWindow.createWindow(this);
		input.setupJoystickInputs();
		Initialize();
	}

	void Graphics2d::onCursorPos(double xPosIn, double yPosIn)
	{
		this->input.onCursorPos(xPosIn, yPosIn);
	}

	void Graphics2d::onFramebufferSizeChange(int width, int height)
	{
		glViewport(0, 0, width, height);
		screenWidth = width;
		screenHeight = height;
	}

	void Graphics2d::onScroll(double xOffset, double yOffset)
	{
		this->input.onScroll(xOffset, yOffset);
	}

	void Graphics2d::onMouseButton(int button, int action, int mods)
	{
		this->input.onMouseButton(button, action, mods);
	}

	void Graphics2d::onKeyInput(int button, int scancode, int action, int mods)
	{
		this->input.onKeyInput(button, scancode, action, mods);
	}

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
		glfwSwapBuffers(window);
	}


	void Graphics2d::Draw(Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color)
	{
		// This one will use a default shader that will already be loaded into graphics
		defaultShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, 0.0f));

		// This transforms the center so you can rotate by the center then transforms back
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

		model = glm::scale(model, glm::vec3(size, 1.0f));

		if (hasCamera)
		{
			defaultShader.setMat4("view", mainCamera.GetViewMatrix());
		}
		else
		{
			defaultShader.setMat4("view", glm::mat4(1.0f));
		}

		defaultShader.setMat4("model", model);
		defaultShader.setVec3("spriteColor", color.GetRGB());

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
		glDeleteFramebuffers(1, &quadVBO);

	}

	void Graphics2d::SetMainCamera(Camera& camera)
	{
		mainCamera = camera;
		hasCamera = true;
	}

}
