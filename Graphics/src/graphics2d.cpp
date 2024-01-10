#include "graphics2d.hpp"

// https://www.glfw.org/docs/3.3/input_guide.html
namespace Ebony
{
	int Graphics2d::activeShaderId{ -1 };
	int Graphics2d::activeTextureId{ -1 };

	Window Graphics2d::window;
	float Graphics2d::lastMosX;
	float Graphics2d::lastMosY;
	std::shared_ptr<Camera> Graphics2d::mainCamera;
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
	bool Graphics2d::resized{ false };
	const char* Graphics2d::windowName;
	int Graphics2d::screenWidth;
	int Graphics2d::screenHeight;
	int Graphics2d::renderWidth;
	int Graphics2d::renderHeight;

	int Graphics2d::bufferDrawing{ 30 }; // This is how much more of the screen to draw, since you will see the updating screen as it goes out of view on the ope and left
	
	ImGuiIO Graphics2d::io;
	glm::mat4 Graphics2d::projection;
	std::priority_queue<DrawableObject, std::vector<DrawableObject>, CompareDrawableDepth> Graphics2d::renderPriorityQueue{};
	std::priority_queue<DrawableObject, std::vector<DrawableObject>, CompareDrawableDepth> Graphics2d::dynamicQueue{};



	void Graphics2d::InitializeImgui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		//ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
		
		ImGui_ImplOpenGL3_Init("#version 330");
	}


	void Graphics2d::Initialize(const char* windowName, int width, int height, int renderWidth, int renderHeight) {
		windowName = windowName;
		Graphics2d::screenWidth = width;
		Graphics2d::screenHeight = height;
		Graphics2d::renderWidth = renderWidth;
		Graphics2d::renderHeight = renderHeight;
		versionMajor = 3;
		versionMinor = 3;

		window = Window();
		window.createWindow(windowName, versionMajor, versionMinor, screenWidth, screenHeight);
		Initialize();
		SetupCallback();
		InitializeImgui();

		projection = glm::ortho(0.0f, static_cast<float>(renderWidth), static_cast<float>(renderHeight), 0.0f, -1.0f, 1.0f);
		initRenderData();
	}

	void Graphics2d::Initialize(const char* windowName, int width, int height, int renderWidth, int renderHeight, int majorVersion, int minorVersion)
	{
		windowName = windowName;
		Graphics2d::screenWidth = width;
		Graphics2d::screenHeight = height;
		Graphics2d::renderWidth = renderWidth;
		Graphics2d::renderHeight = renderHeight;
		versionMajor = majorVersion;
		versionMinor = minorVersion;

		window = Window();
		window.createWindow(versionMajor, versionMinor, screenWidth, screenHeight);
		SetupCallback();
		Initialize();

		projection = glm::ortho(0.0f, static_cast<float>(renderWidth), static_cast<float>(renderHeight), 0.0f, -1.0f, 1.0f);
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

	void Graphics2d::setNewWindowSize(int width, int height)
	{
		screenHeight = height;
		screenWidth = width;
		SDL_SetWindowSize(window.getWindow(), width, height);
		glViewport(0, 0, width, height);
		resized = true;
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
		//ImGui_ImplGlfw_NewFrame();
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
		activeShaderId = -1;
		SDL_GL_SwapWindow(window.getWindow());
		resized = false;
	}


	void Graphics2d::Draw(std::shared_ptr<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 rotationAxis, Color color, float depth, bool isUI, bool isSpriteSheet, std::uint64_t layer)
	{
		// This one will use a default shader that will already be loaded into graphics
		std::shared_ptr<Shader> s = ResourceManager::GetShader("default");

		// Check to make sure it can even see if for drawing

		if (Graphics2d::hasCamera)
		{
			if (isUI || Graphics2d::mainCamera->Position.x - bufferDrawing < position.x && Graphics2d::mainCamera->Position.x + screenWidth > position.x &&
				Graphics2d::mainCamera->Position.y - bufferDrawing < position.y && Graphics2d::mainCamera->Position.y + screenHeight > position.y)
			{
				DrawableObject drawableObject = DrawableObject(s, texture, position, size, rotate, rotationAxis, color, depth, isUI, isSpriteSheet, layer);
				Graphics2d::renderPriorityQueue.push(drawableObject);
			}
		}
		else
		{
			DrawableObject drawableObject = DrawableObject(s, texture, position, size, rotate, rotationAxis, color, depth, isUI, isSpriteSheet, layer);
			Graphics2d::renderPriorityQueue.push(drawableObject);
		}
	}

	void Graphics2d::Draw(std::shared_ptr<Shader> s, std::shared_ptr<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 rotationAxis, Color color, float depth, bool isUI, bool isSpriteSheet, std::uint64_t layer)
	{
		if (Graphics2d::hasCamera)
		{
			if (isUI || Graphics2d::mainCamera->Position.x - bufferDrawing < position.x && Graphics2d::mainCamera->Position.x + screenWidth > position.x &&
				Graphics2d::mainCamera->Position.y - bufferDrawing < position.y && Graphics2d::mainCamera->Position.y + screenHeight > position.y)
			{
				DrawableObject drawableObject = DrawableObject(s, texture, position, size, rotate, rotationAxis, color, depth, isUI, isSpriteSheet, layer);
				Graphics2d::renderPriorityQueue.push(drawableObject);
			}
		}
		else
		{
			DrawableObject drawableObject = DrawableObject(s, texture, position, size, rotate, rotationAxis, color, depth, isUI, isSpriteSheet, layer);
			Graphics2d::renderPriorityQueue.push(drawableObject);
		}
	}

	void Graphics2d::DrawAnimation(std::shared_ptr<Shader> s, std::shared_ptr<Texture2D> texture, std::uint16_t layer, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 rotationAxis, Color color, float depth, bool isUI)
	{
		if (Graphics2d::hasCamera)
		{
			if (isUI || Graphics2d::mainCamera->Position.x - bufferDrawing < position.x && Graphics2d::mainCamera->Position.x + screenWidth > position.x &&
				Graphics2d::mainCamera->Position.y - bufferDrawing < position.y && Graphics2d::mainCamera->Position.y + screenHeight > position.y)
			{
				DrawableObject drawableObject = DrawableObject(s, texture, position, size, rotate, rotationAxis, color, depth, isUI, true, layer);
				Graphics2d::renderPriorityQueue.push(drawableObject);
			}
		}
		else
		{
			DrawableObject drawableObject = DrawableObject(s, texture, position, size, rotate, rotationAxis, color, depth, isUI, true, layer);
			Graphics2d::renderPriorityQueue.push(drawableObject);
		}

		//if (activeShaderId != s->ID)
		//{
		//	s->use();
		//	activeShaderId = s->ID;
		//	//s->setMat4("projection", glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f));
		//}
		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(position, depth));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

		//model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f)); // move origin of rotation to center of quad
		//model = glm::rotate(model, glm::radians(rotate), rotationAxis); // then rotate
		//model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // move origin back
		//model = glm::scale(model, glm::vec3(size, 1.0f)); // last scale

		//if (!isUI && hasCamera)
		//{
		//	s->setMat4("view", mainCamera->GetViewMatrix());
		//}
		//else
		//{
		//	s->setMat4("view", glm::mat4(1.0f));
		//}

		//s->setMat4("model", model);
		//s->setInt("layer", layer);

		//if (activeTextureId != texture->ID)
		//{
		//	glActiveTexture(GL_TEXTURE0);
		//	texture->Bind();
		//	activeTextureId = texture->ID;
		//}



		//glBindVertexArray(quadVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);
	}


	void Graphics2d::DrawInstanced(std::shared_ptr<Shader> s, std::shared_ptr<Texture2D> texture, unsigned int VAO, std::uint32_t count)
	{
		if (activeShaderId != s->ID)
		{
			s->use();
			activeShaderId = s->ID;
		}
		if (activeTextureId != texture->ID)
		{
			glActiveTexture(GL_TEXTURE0);
			texture->Bind();
			activeTextureId = texture->ID;
		}

		if (hasCamera)
		{
			s->setMat4("view", mainCamera->GetViewMatrix());
		}
		else
		{
			s->setMat4("view", glm::mat4(1.0f));
		}

		glBindVertexArray(VAO);
		// TODO: Convert this to GL_TRIANGLE_STRIP
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, count);
		glBindVertexArray(0);
	}

	void Graphics2d::DrawFromQueue()
	{
		while (Graphics2d::renderPriorityQueue.size() > 0)
		{
			DrawableObject drawable = Graphics2d::renderPriorityQueue.top();
			Graphics2d::renderPriorityQueue.pop();

			if (activeShaderId != drawable.s->ID)
			{
				drawable.s->use();
				activeShaderId = drawable.s->ID;
				//drawable.s->setMat4("projection", glm::ortho(0.0f, static_cast<float>(renderWidth), static_cast<float>(renderHeight), 0.0f));
			}

			//glEnable(GL_TEXTURE_2D_ARRAY);

			if (drawable.isString)
			{
				drawable.s->setVec3("textColor", drawable.color.GetRGB());

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(drawable.position, drawable.depth));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

				model = glm::translate(model, glm::vec3(0.5f * drawable.size.x, 0.5f * drawable.size.y, 0.0f)); // move origin of rotation to center of quad
				model = glm::rotate(model, glm::radians(drawable.rotate), drawable.rotationAxis); // then rotate
				model = glm::translate(model, glm::vec3(-0.5f * drawable.size.x, -0.5f * drawable.size.y, 0.0f)); // move origin back
				model = glm::scale(model, glm::vec3(drawable.size, 1.0f)); // last scale

				if (!drawable.isUI && hasCamera)
				{
					drawable.s->setMat4("view", mainCamera->GetViewMatrix());
				}
				else
				{
					drawable.s->setMat4("view", glm::mat4(1.0f));
				}

				drawable.s->setMat4("model", model);

				glActiveTexture(GL_TEXTURE0);
				glBindVertexArray(textVAO);

				// Iterate through all characters
				std::string::const_iterator c;
				for (c = drawable.text.begin(); c != drawable.text.end(); c++)
				{
					Character ch = drawable.spriteFont->characters[*c];

					float xpos = drawable.position.x + ch.Bearing.x * drawable.size.x;
					float ypos = drawable.position.y - (ch.Size.y - ch.Bearing.y) * drawable.size.y;

					float w = ch.Size.x * drawable.size.x;
					float h = ch.Size.y * drawable.size.y;

					// Update VBO for each character
					float vertices[6][4] = {
						{ xpos,         ypos,					0.0f, 0.0f  },
						{ xpos,         ypos + h,				0.0f, 1.0f  },
						{ xpos + w,     ypos + h,				1.0f, 1.0f  },
						{ xpos,         ypos,					0.0f, 0.0f  },
						{ xpos + w,     ypos + h,				1.0f, 1.0f  },
						{ xpos + w,     ypos,					1.0f, 0.0f  }
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
					drawable.position.x += (ch.Advance >> 6) * drawable.size.x; // Bitshift by 6 to get value in pixels (2^6 = 64)
				}

				glBindVertexArray(0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else if (drawable.isInstanced)
			{

			}
			else
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(drawable.position, drawable.depth));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)

				model = glm::translate(model, glm::vec3(0.5f * drawable.size.x, 0.5f * drawable.size.y, 0.0f)); // move origin of rotation to center of quad
				model = glm::rotate(model, glm::radians(drawable.rotate), drawable.rotationAxis); // then rotate
				model = glm::translate(model, glm::vec3(-0.5f * drawable.size.x, -0.5f * drawable.size.y, 0.0f)); // move origin back
				model = glm::scale(model, glm::vec3(drawable.size, 1.0f)); // last scale

				if (!drawable.isUI && hasCamera)
				{
					drawable.s->setMat4("view", mainCamera->GetViewMatrix());
				}
				else
				{
					drawable.s->setMat4("view", glm::mat4(1.0f));
				}

				drawable.s->setMat4("model", model);
				drawable.s->setVec4("spriteColor", drawable.color.GetRGBA());

				if (drawable.isSpriteSheet)
				{
					//s->setInt("spritesheet", texture->ID);
					drawable.s->setInt("layer", static_cast<int>(drawable.layer));
				}



				if (activeTextureId != drawable.texture->ID)
				{
					glActiveTexture(GL_TEXTURE0);
					drawable.texture->Bind();
					activeTextureId = drawable.texture->ID;
				}



				glBindVertexArray(quadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}
			
			//glDisable(GL_TEXTURE_2D_ARRAY);
		}
	}

	void Graphics2d::DrawRenderTarget(std::shared_ptr<Shader> s, RenderTarget2D& renderTarget)
	{
		s->use();
		glBindVertexArray(quadRenderTarget);
		glBindTexture(GL_TEXTURE_2D, renderTarget.GetTextureColorBuffer());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void Graphics2d::DrawString(std::shared_ptr<Shader> s, std::shared_ptr<SpriteFont> spriteFont, std::string text, glm::vec2 position, glm::vec2 size, glm::vec2 transformScale, float rotate, glm::vec3 rotationAxis, Color color, Color outlineColor, float depth, bool isUI)
	{
		DrawableObject drawableObject = DrawableObject(s, nullptr, position, size, rotate, rotationAxis, color, depth, isUI, true, 0, true, text, spriteFont, false);
		Graphics2d::renderPriorityQueue.push(drawableObject);
	}

	void Graphics2d::InitializeTextDrawing(std::shared_ptr<Shader> textShader)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// TODO: This should change to use the actual window sizes
		//glm::mat4 textProjection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));
		textShader->use();
		textShader->setMat4("projection", glm::ortho(0.0f, static_cast<float>(renderWidth), static_cast<float>(renderHeight), 0.0f));

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
		//ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();


		glDeleteVertexArrays(1, &quadVAO);
	}

	void Graphics2d::SetMainCamera(std::shared_ptr<Camera> camera)
	{
		mainCamera = camera;
		hasCamera = true;
	}

	void Graphics2d::SetRenderTarget(RenderTarget2D& renderTarget, Color clearColor)
	{
		//if (Graphics2d::resized)
		//{
		//	renderTarget.Resize(Graphics2d::screenWidth, Graphics2d::screenHeight);
		//}

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
