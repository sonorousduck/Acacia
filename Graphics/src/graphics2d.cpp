#include "graphics2d.hpp"
#include "window.hpp"
#include "resourceManager.hpp"
#include "spritefont.hpp"


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

		window = Window();
		window.createWindow(windowName, versionMajor, versionMinor);
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

		window = Window();
		window.createWindow(versionMajor, versionMinor);
		SetupCallback();
		Initialize();
	}

	void Graphics2d::SetupCallback()
	{
		auto framebuffer_callback = [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onFramebufferSizeChange(width, height);
		};
		
		glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_callback);
	}

	void Graphics2d::onFramebufferSizeChange(int width, int height)
	{
		screenWidth = width;
		screenHeight = height;
	}

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

	void Graphics2d::DrawString(Shader& s, SpriteFont& spriteFont, std::string text, float x, float y, float scale, Color color)
	{
		s.use();
		s.setVec3("textColor", color.GetRGB());
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(textVAO);

		// Iterate through all characters
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = spriteFont.characters[*c];

			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			// Update VBO for each character
			float vertices[6][4] = {
				{ xpos,         ypos + h,   0.0f, 0.0f  },
				{ xpos,         ypos,       0.0f, 1.0f  },
				{ xpos + w,     ypos,       1.0f, 1.0f  },
				{ xpos,         ypos + h,   0.0f, 0.0f  },
				{ xpos + w,     ypos,       1.0f, 1.0f  },
				{ xpos + w,     ypos + h,   1.0f, 0.0f  }
			};

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Render Quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Now advance cursors for next glyph (Note that advance number is 1/64 pixels)
			x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);


	}

	//void Graphics2d::DrawString(SpriteFont& spriteFont, std::string text, float x, float y, float scale, glm::vec3 color)
	//{



	//}



	void Graphics2d::InitializeTextDrawing(Shader& textShader)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 textProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
		textShader.use();
		textShader.setMat4("projection", textProjection);

		glGenVertexArrays(1, &textVAO);
		glGenBuffers(1, &textVBO);
		glBindVertexArray(textVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW); // It needs 6 vertices of 4 floats each, so 6 * 4 floats of memory (x, y, u, v)

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	

	void Graphics2d::Cleanup()
	{
		glDeleteVertexArrays(1, &quadVAO);
	}

	void Graphics2d::SetMainCamera(Camera& camera)
	{
		mainCamera = camera;
		hasCamera = true;
	}

}
