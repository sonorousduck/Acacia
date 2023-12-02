#include "graphics2d.hpp"
#include "../Ebony/src/misc/resourceManager.hpp"

// https://www.glfw.org/docs/3.3/input_guide.html
namespace Ebony
{
	int Graphics2d::activeShaderId{ -1 };
	int Graphics2d::activeTextureId{ -1 };

	Window Graphics2d::window;
	float Graphics2d::lastMosX;
	float Graphics2d::lastMosY;
	Camera Graphics2d::mainCamera;
	unsigned int Graphics2d::versionMajor;
	unsigned int Graphics2d::versionMinor;
	unsigned int Graphics2d::quadVAO{ 0 };
	unsigned int Graphics2d::instancedVAO{ 0 };
	unsigned int Graphics2d::quadRenderTarget{ 0 };
	unsigned int Graphics2d::particlePositionBuffer{ 0 };
	unsigned int Graphics2d::particleColorBuffer{ 0 };
	unsigned int Graphics2d::textVAO{ 0 };
	unsigned int Graphics2d::textVBO{ 0 };
	bool Graphics2d::firstMouse{ true };
	bool Graphics2d::cursorDisabled{ true };
	bool Graphics2d::hasCamera{ false };

	const char* Graphics2d::windowName;
	int Graphics2d::screenWidth;
	int Graphics2d::screenHeight;
	ImGuiIO Graphics2d::io;
	glm::mat4 Graphics2d::projection;


	void Graphics2d::InitializeImgui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		//ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
		
		ImGui_ImplOpenGL3_Init("#version 330");
	}


	void Graphics2d::Initialize(const char* windowName, int width, int height) {
		windowName = windowName;
		screenWidth = width;
		screenHeight = height;
		versionMajor = 3;
		versionMinor = 3;

		window = Window();
		window.createWindow(windowName, versionMajor, versionMinor, screenWidth, screenHeight);
		Initialize();
		SetupCallback();
		InitializeImgui();

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
		window.createWindow(versionMajor, versionMinor, screenWidth, screenHeight);
		SetupCallback();
		Initialize();
	}

	void Graphics2d::SetupCallback()
	{
		/*auto framebuffer_callback = [](GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
			static_cast<Graphics2d*>(glfwGetWindowUserPointer(window))->onFramebufferSizeChange(width, height);
		};
		
		glfwSetFramebufferSizeCallback(window.getWindow(), framebuffer_callback);*/
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


		/*glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		glfwMakeContextCurrent(window.getWindow());*/

		/*if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}*/
	}

	void Graphics2d::initRenderData()
	{
		// Since all sprites share the same vertex data, you only need one of these
		unsigned int VBO = 0;
		float vertices[] = {
			// pos      // tex
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		unsigned int VBORenderTarget;
		float verticesRenderTarget[] = {
			// pos      // tex
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		glGenVertexArrays(1, &quadRenderTarget);
		glGenBuffers(1, &VBORenderTarget);

		glBindVertexArray(quadRenderTarget);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBORenderTarget);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRenderTarget), &verticesRenderTarget, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	void Graphics2d::BeginDraw(Color clearColor)
	{

		glClearColor(clearColor.r(), clearColor.g(), clearColor.b(), clearColor.a());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	}

	void Graphics2d::BeginImgui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();
	}

	void Graphics2d::DrawWindow(std::string_view windowName)
	{
		ImGui::Begin(windowName.data());
	}

	void Graphics2d::ImguiText(std::string_view text)
	{
		ImGui::Text(text.data());
	}

	void Graphics2d::CompleteWindow()
	{
		ImGui::End();
	}

	void Graphics2d::EndImgui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


	void Graphics2d::EndDraw()
	{
		activeTextureId = -1;
		SDL_GL_SwapWindow(window.getWindow());
		//glfwSwapBuffers(window.getWindow());
	}


	void Graphics2d::Draw(const Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color, float depth)
	{
		// This one will use a default shader that will already be loaded into graphics
		Shader& s = ResourceManager::GetShader("default");
		if (activeShaderId != s.ID)
		{
			s.use();
			activeShaderId = s.ID;
		}

		glEnable(GL_TEXTURE_2D_ARRAY);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, depth));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

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

		if (activeTextureId != texture.ID)
		{
			glActiveTexture(GL_TEXTURE0);
			texture.Bind();
			activeTextureId = texture.ID;
		}

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glDisable(GL_TEXTURE_2D_ARRAY);

	}

	void Graphics2d::Draw(Shader& s, Texture2D& texture, glm::vec2 position, glm::vec2 size, float rotate, Color color, float depth)
	{
		if (activeShaderId != s.ID) 
		{
			s.use();
			activeShaderId = s.ID;
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, depth));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

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
		s.setVec3("spriteColor", color.GetRGB());
		//s.setVec3("spriteColor", Colors::White.GetRGB());
		//s.setInt("spritesheet", texture.ID);

		if (activeTextureId != texture.ID)
		{
			glActiveTexture(GL_TEXTURE0);
			texture.Bind();
			activeTextureId = texture.ID;
		}



		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Graphics2d::DrawAnimation(Shader& s, Texture2D& texture, std::uint16_t layer, glm::vec2 position, glm::vec2 size, float rotate, Color color, float depth)
	{
		if (activeShaderId != s.ID)
		{
			s.use();
			activeShaderId = s.ID;
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(position, depth));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

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
		s.setInt("layer", layer);

		if (activeTextureId != texture.ID)
		{
			glActiveTexture(GL_TEXTURE0);
			texture.Bind();
			activeTextureId = texture.ID;
		}



		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}


	void Graphics2d::DrawInstanced(Shader& s, Texture2D& texture, unsigned int VAO, std::uint32_t count)
	{
		if (activeShaderId != s.ID)
		{
			s.use();
			activeShaderId = s.ID;
		}
		if (activeTextureId != texture.ID)
		{
			glActiveTexture(GL_TEXTURE0);
			texture.Bind();
			activeTextureId = texture.ID;
		}

		if (hasCamera)
		{
			s.setMat4("view", mainCamera.GetViewMatrix());
		}
		else
		{
			s.setMat4("view", glm::mat4(1.0f));
		}

		glBindVertexArray(VAO);
		// TODO: Convert this to GL_TRIANGLE_STRIP
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
		glBindVertexArray(0);
	}


	void Graphics2d::DrawRenderTarget(Shader& s, RenderTarget2D& renderTarget)
	{
		s.use();
		glBindVertexArray(quadRenderTarget);
		glBindTexture(GL_TEXTURE_2D, renderTarget.GetTextureColorBuffer());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Graphics2d::DrawString(Shader& s, SpriteFont& spriteFont, std::string text, float x, float y, float scale, Color color, Color outlineColor, float depth)
	{
		if (activeShaderId != s.ID)
		{
			s.use();
			activeShaderId = s.ID;
		}

		s.setVec3("textColor", color.GetRGB());
		s.setVec3("outlineColor", outlineColor.GetRGB());
		s.setFloat("depth", depth);

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

	void Graphics2d::InitializeTextDrawing(Shader& textShader)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// TODO: This should change to use the actual window sizes
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
		// This probably won't be where IMGUI ends up, since we will want to be able to disable it when it is running a game
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();


		glDeleteVertexArrays(1, &quadVAO);
	}

	void Graphics2d::SetMainCamera(Camera& camera)
	{
		mainCamera = camera;
		hasCamera = true;
	}

	void Graphics2d::SetRenderTarget(RenderTarget2D& renderTarget, Color clearColor)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.GetFramebuffer());
		glClearColor(clearColor.r(), clearColor.g(), clearColor.b(), clearColor.a());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Graphics2d::UnbindRenderTarget(Color clearColor)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClearColor(clearColor.r(), clearColor.g(), clearColor.b(), clearColor.a());
		//glClear(GL_COLOR_BUFFER_BIT);
	}

}
