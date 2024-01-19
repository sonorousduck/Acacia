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
	bool Graphics2d::textInitialized{ false };

	const char* Graphics2d::windowName;
	int Graphics2d::screenWidth;
	int Graphics2d::screenHeight;
	int Graphics2d::renderWidth;
	int Graphics2d::renderHeight;

	int Graphics2d::bufferDrawing{ 30 }; // This is how much more of the screen to draw, since you will see the updating screen as it goes out of view on the ope and left
	
	ImGuiIO Graphics2d::io;
	glm::mat4 Graphics2d::projection;
	std::priority_queue<DrawableObject, std::vector<DrawableObject>, CompareDrawableDepth> Graphics2d::renderPriorityQueue{};


	std::vector<DrawableObject> Graphics2d::backgroundQueue{};
	std::vector<DrawableObject> Graphics2d::nearBackgroundQueue{};
	std::vector<DrawableObject> Graphics2d::foregroundQueue{};
	std::vector<DrawableObject> Graphics2d::uiQueue{};
	std::vector<DrawableObject> Graphics2d::alwaysFrontQueue{};

	float Graphics2d::BACKGROUND_DEPTH{ 0.01f };
	float Graphics2d::NEAR_BACKGROUND_DEPTH{ 0.05f };
	float Graphics2d::FOREGROUND_DEPTH{ 0.50f };
	float Graphics2d::UI_DEPTH{ 0.70f };
	float Graphics2d::ALWAYS_FRONT_DEPTH{ 0.99f };



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
		//InitializeImgui();

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


	void Graphics2d::Draw(entities::EntityPtr entity, bool isSpriteSheet)
	{
		//std::shared_ptr<Texture2D> texture, glm::vec2 position, glm::vec2 size, float rotate, glm::vec3 rotationAxis, Color color, float depth, bool isUI, bool isSpriteSheet, std::uint64_t layer
		// This one will use a default shader that will already be loaded into graphics
		//std::shared_ptr<Shader> s = ResourceManager::GetShader("default");

		auto sprite = entity->getComponent<components::Sprite>();
		auto transform = entity->getComponent<components::Transform>();
		isSpriteSheet = sprite->isAtlas;
		// Check to make sure it can even see if for drawing

		if (Graphics2d::hasCamera)
		{
			if (!(Graphics2d::mainCamera->Position.x - bufferDrawing < transform->position.x && Graphics2d::mainCamera->Position.x + screenWidth > transform->position.x &&
				Graphics2d::mainCamera->Position.y - bufferDrawing < transform->position.y && Graphics2d::mainCamera->Position.y + screenHeight > transform->position.y) && !sprite->isUI)
			{
				return;
			}
		}

		switch (sprite->renderLayer)
		{
		case (Ebony::RenderLayer::BACKGROUND):
		{
			DrawableObject drawableObject = DrawableObject(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, transform->rotationAxis, sprite->spriteColor, sprite->isUI, isSpriteSheet, sprite->layer, Graphics2d::BACKGROUND_DEPTH);
			Graphics2d::backgroundQueue.push_back(drawableObject);
			return;
		}
		case (Ebony::RenderLayer::NEAR_BACKGROUND):
		{
			DrawableObject drawableObject = DrawableObject(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, transform->rotationAxis, sprite->spriteColor, sprite->isUI, isSpriteSheet, sprite->layer, Graphics2d::NEAR_BACKGROUND_DEPTH);
			Graphics2d::nearBackgroundQueue.push_back(drawableObject);
			return;
		}
				
		case (Ebony::RenderLayer::FOREGROUND):
		{
			DrawableObject drawableObject = DrawableObject(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, transform->rotationAxis, sprite->spriteColor, sprite->isUI, isSpriteSheet, sprite->layer, Graphics2d::FOREGROUND_DEPTH);
			Graphics2d::foregroundQueue.push_back(drawableObject);
			return;
		}
				
		case (Ebony::RenderLayer::UI_RENDER):
		{
			DrawableObject drawableObject = DrawableObject(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, transform->rotationAxis, sprite->spriteColor, sprite->isUI, isSpriteSheet, sprite->layer, Graphics2d::UI_DEPTH);
			Graphics2d::uiQueue.push_back(drawableObject);
			return;
		}
				
		case (Ebony::RenderLayer::ALWAYS_FRONT):
		{
			DrawableObject drawableObject = DrawableObject(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, transform->rotationAxis, sprite->spriteColor, sprite->isUI, isSpriteSheet, sprite->layer, Graphics2d::ALWAYS_FRONT_DEPTH);
			Graphics2d::alwaysFrontQueue.push_back(drawableObject);
			return;
		}
				
		case (Ebony::RenderLayer::DYNAMIC_PLACING):
		{
			DrawableObject drawableObject = DrawableObject(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, transform->rotationAxis, sprite->spriteColor, sprite->isUI, isSpriteSheet, sprite->layer, sprite->depth);
			Graphics2d::renderPriorityQueue.push(drawableObject);
			return;
		}
		}
	}

	void Graphics2d::DrawAnimation(entities::EntityPtr entity)
	{
		auto animationController = entity->getComponent<components::AnimationController>();
		auto transform = entity->getComponent<components::Transform>();
		auto animationsToDraw = animationController->GetSprite();


		if (Graphics2d::hasCamera)
		{
			if (!(Graphics2d::mainCamera->Position.x - bufferDrawing < transform->position.x && Graphics2d::mainCamera->Position.x + screenWidth > transform->position.x &&
				Graphics2d::mainCamera->Position.y - bufferDrawing < transform->position.y && Graphics2d::mainCamera->Position.y + screenHeight > transform->position.y) && !animationController->isUI)
			{
				return;
			}
		}
		
		for (auto& animation : animationsToDraw)
		{
			switch (animationController->renderLayer)
			{
			case (Ebony::RenderLayer::BACKGROUND):
			{
				DrawableObject drawableObject = DrawableObject(animation.shader, animation.GetSprite().spritesheet, transform->position, transform->scale, transform->rotation, transform->rotationAxis, animation.GetSpriteColor(), animationController->isUI, true, animation.GetCurrentSpriteFrame(), Graphics2d::BACKGROUND_DEPTH);
				Graphics2d::backgroundQueue.push_back(drawableObject);
				return;
			}
			case (Ebony::RenderLayer::NEAR_BACKGROUND):
			{
				DrawableObject drawableObject = DrawableObject(animation.shader, animation.GetSprite().spritesheet, transform->position, transform->scale, transform->rotation, transform->rotationAxis, animation.GetSpriteColor(), animationController->isUI, true, animation.GetCurrentSpriteFrame(), Graphics2d::NEAR_BACKGROUND_DEPTH);
				Graphics2d::nearBackgroundQueue.push_back(drawableObject);
				return;
			}

			case (Ebony::RenderLayer::FOREGROUND):
			{
				DrawableObject drawableObject = DrawableObject(animation.shader, animation.GetSprite().spritesheet, transform->position, transform->scale, transform->rotation, transform->rotationAxis, animation.GetSpriteColor(), animationController->isUI, true, animation.GetCurrentSpriteFrame(), Graphics2d::FOREGROUND_DEPTH);
				Graphics2d::foregroundQueue.push_back(drawableObject);
				return;
			}

			case (Ebony::RenderLayer::UI_RENDER):
			{
				DrawableObject drawableObject = DrawableObject(animation.shader, animation.GetSprite().spritesheet, transform->position, transform->scale, transform->rotation, transform->rotationAxis, animation.GetSpriteColor(), animationController->isUI, true, animation.GetCurrentSpriteFrame(), Graphics2d::UI_DEPTH);
				Graphics2d::uiQueue.push_back(drawableObject);
				return;
			}

			case (Ebony::RenderLayer::ALWAYS_FRONT):
			{
				DrawableObject drawableObject = DrawableObject(animation.shader, animation.GetSprite().spritesheet, transform->position, transform->scale, transform->rotation, transform->rotationAxis, animation.GetSpriteColor(), animationController->isUI, true, animation.GetCurrentSpriteFrame(), Graphics2d::ALWAYS_FRONT_DEPTH);
				Graphics2d::alwaysFrontQueue.push_back(drawableObject);
				return;
			}

			case (Ebony::RenderLayer::DYNAMIC_PLACING):
			{
				DrawableObject drawableObject = DrawableObject(animation.shader, animation.GetSprite().spritesheet, transform->position, transform->scale, transform->rotation, transform->rotationAxis, animation.GetSpriteColor(), animationController->isUI, true, animation.GetCurrentSpriteFrame(), animation.GetDepth());
				Graphics2d::renderPriorityQueue.push(drawableObject);
				return;
			}
			}
		}
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
		while (Graphics2d::renderPriorityQueue.size() > 0 && Graphics2d::renderPriorityQueue.top().depth < Graphics2d::BACKGROUND_DEPTH)
		{
			DrawableObject dynamicDrawable = Graphics2d::renderPriorityQueue.top();
			Graphics2d::InternalDraw(dynamicDrawable);
			Graphics2d::renderPriorityQueue.pop();
		}



		// Draw background first. Nothing will ever be behind this.
		for (auto i = 0; i < Graphics2d::backgroundQueue.size(); i++)
		{
			Graphics2d::InternalDraw(Graphics2d::backgroundQueue[i]);
		}


		while (Graphics2d::renderPriorityQueue.size() > 0 && Graphics2d::renderPriorityQueue.top().depth < Graphics2d::NEAR_BACKGROUND_DEPTH)
		{
			DrawableObject dynamicDrawable = Graphics2d::renderPriorityQueue.top();
			Graphics2d::InternalDraw(dynamicDrawable);
			Graphics2d::renderPriorityQueue.pop();
		}

		// Draw near background next
		for (auto i = 0; i < Graphics2d::nearBackgroundQueue.size(); i++)
		{
			Graphics2d::InternalDraw(Graphics2d::nearBackgroundQueue[i]);
		}

		while (Graphics2d::renderPriorityQueue.size() > 0 && Graphics2d::renderPriorityQueue.top().depth < Graphics2d::FOREGROUND_DEPTH)
		{
			DrawableObject dynamicDrawable = Graphics2d::renderPriorityQueue.top();
			Graphics2d::InternalDraw(dynamicDrawable);
			Graphics2d::renderPriorityQueue.pop();
		}

		// Draw Foreground
		for (auto i = 0; i < Graphics2d::foregroundQueue.size(); i++)
		{
			Graphics2d::InternalDraw(Graphics2d::foregroundQueue[i]);
		}

		while (Graphics2d::renderPriorityQueue.size() > 0 && Graphics2d::renderPriorityQueue.top().depth < Graphics2d::UI_DEPTH)
		{
			DrawableObject dynamicDrawable = Graphics2d::renderPriorityQueue.top();
			Graphics2d::InternalDraw(dynamicDrawable);
			Graphics2d::renderPriorityQueue.pop();
		}

		// Draw UI
		for (auto i = 0; i < Graphics2d::uiQueue.size(); i++)
		{
			Graphics2d::InternalDraw(Graphics2d::uiQueue[i]);
		}
			
		
		while (Graphics2d::renderPriorityQueue.size() > 0 && Graphics2d::renderPriorityQueue.top().depth < Graphics2d::ALWAYS_FRONT_DEPTH)
		{
			DrawableObject dynamicDrawable = Graphics2d::renderPriorityQueue.top();
			Graphics2d::InternalDraw(dynamicDrawable);
			Graphics2d::renderPriorityQueue.pop();
		}

		// Draw always on top items
		for (auto i = 0; i < Graphics2d::alwaysFrontQueue.size(); i++)
		{
			Graphics2d::InternalDraw(Graphics2d::alwaysFrontQueue[i]);
		}

		while (Graphics2d::renderPriorityQueue.size() > 0)
		{
			DrawableObject dynamicDrawable = Graphics2d::renderPriorityQueue.top();
			Graphics2d::InternalDraw(dynamicDrawable);
			Graphics2d::renderPriorityQueue.pop();
		}


		Graphics2d::backgroundQueue.clear();
		Graphics2d::nearBackgroundQueue.clear();
		Graphics2d::foregroundQueue.clear();
		Graphics2d::uiQueue.clear();
		Graphics2d::alwaysFrontQueue.clear();
	}

	void Graphics2d::InternalDraw(DrawableObject drawable)
	{
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
	}

	void Graphics2d::DrawRenderTarget(std::shared_ptr<Shader> s, RenderTarget2D& renderTarget)
	{
		s->use();
		glBindVertexArray(quadRenderTarget);
		glBindTexture(GL_TEXTURE_2D, renderTarget.GetTextureColorBuffer());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	// Transform modification is used for outlining the text
	void Graphics2d::DrawString(entities::EntityPtr entity, glm::vec2 transformModification, bool isOutline)
	{
		auto transform = entity->getComponent<components::Transform>();
		auto text = entity->getComponent<components::Text>();


		if (Graphics2d::hasCamera)
		{
			if (!(Graphics2d::mainCamera->Position.x - bufferDrawing < transform->position.x && Graphics2d::mainCamera->Position.x + screenWidth > transform->position.x &&
				Graphics2d::mainCamera->Position.y - bufferDrawing < transform->position.y && Graphics2d::mainCamera->Position.y + screenHeight > transform->position.y) && !text->isUI)
			{
				return;
			}
		}

		switch (text->renderLayer)
		{
		case (Ebony::RenderLayer::BACKGROUND):
		{
			DrawableObject drawableObject = DrawableObject(text->shader, nullptr, transform->position + transformModification, transform->scale, transform->rotation, transform->rotationAxis, isOutline ? text->color : text->outlineColor, Graphics2d::BACKGROUND_DEPTH, text->isUI, true, 0, true, text->text, text->spriteFont, false);
			Graphics2d::backgroundQueue.push_back(drawableObject);
			return;
		}
		case (Ebony::RenderLayer::NEAR_BACKGROUND):
		{
			DrawableObject drawableObject = DrawableObject(text->shader, nullptr, transform->position + transformModification, transform->scale, transform->rotation, transform->rotationAxis, isOutline ? text->color : text->outlineColor, Graphics2d::NEAR_BACKGROUND_DEPTH, text->isUI, true, 0, true, text->text, text->spriteFont, false);
			Graphics2d::nearBackgroundQueue.push_back(drawableObject);
			return;
		}

		case (Ebony::RenderLayer::FOREGROUND):
		{
			DrawableObject drawableObject = DrawableObject(text->shader, nullptr, transform->position + transformModification, transform->scale, transform->rotation, transform->rotationAxis, isOutline ? text->color : text->outlineColor, Graphics2d::FOREGROUND_DEPTH, text->isUI, true, 0, true, text->text, text->spriteFont, false);
			Graphics2d::foregroundQueue.push_back(drawableObject);
			return;
		}

		case (Ebony::RenderLayer::UI_RENDER):
		{
			DrawableObject drawableObject = DrawableObject(text->shader, nullptr, transform->position + transformModification, transform->scale, transform->rotation, transform->rotationAxis, isOutline ? text->color : text->outlineColor, Graphics2d::UI_DEPTH, text->isUI, true, 0, true, text->text, text->spriteFont, false);
			Graphics2d::uiQueue.push_back(drawableObject);
			return;
		}

		case (Ebony::RenderLayer::ALWAYS_FRONT):
		{
			DrawableObject drawableObject = DrawableObject(text->shader, nullptr, transform->position + transformModification, transform->scale, transform->rotation, transform->rotationAxis, isOutline ? text->color : text->outlineColor, Graphics2d::ALWAYS_FRONT_DEPTH, text->isUI, true, 0, true, text->text, text->spriteFont, false);
			Graphics2d::alwaysFrontQueue.push_back(drawableObject);
			return;
		}

		case (Ebony::RenderLayer::DYNAMIC_PLACING):
		{
			DrawableObject drawableObject = DrawableObject(text->shader, nullptr, transform->position + transformModification, transform->scale, transform->rotation, transform->rotationAxis, isOutline ? text->color : text->outlineColor, text->layerDepth, text->isUI, true, 0, true, text->text, text->spriteFont, false);
			Graphics2d::renderPriorityQueue.push(drawableObject);
			return;
		}
		}
	}

	void Graphics2d::InitializeTextDrawing(std::shared_ptr<Shader> textShader)
	{
		if (!Graphics2d::textInitialized)
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

			Graphics2d::textInitialized = true;
		}
	}
	

	void Graphics2d::Cleanup()
	{
		// This probably won't be where IMGUI ends up, since we will want to be able to disable it when it is running a game
		//ImGui_ImplOpenGL3_Shutdown();
		//ImGui_ImplGlfw_Shutdown();
		//ImGui::DestroyContext();

		if (quadVAO != 0)
		{
			glDeleteVertexArrays(1, &quadVAO);
		}

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
