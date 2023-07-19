#include "Ebony.hpp"
#include "camera.hpp"


#include "glm/glm.hpp"
#include <resourceManager.hpp>
#include <filesystem>
#include <spritefont.hpp>



namespace Ebony {

	class Sandbox : public Application
	{
	public:
		Sandbox()
		{

		}

		~Sandbox()
		{
			graphics.Cleanup();

		}

		void Init() override
		{
			Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

			graphics.Initialize("Ebony", 800, 600);
			graphics.SetMainCamera(camera);
			Ebony::KeyInput::setupKeyInputs(graphics.window);

			std::vector<int> keys = { GLFW_KEY_E, GLFW_KEY_ESCAPE };
			keyInput.setKeysToMonitorInit(keys);


            Texture2D& faceTexture = ResourceManager::LoadTexture("../Graphics/textures/awesomeface.tx", "face");
            Shader& s = ResourceManager::LoadShader("../Graphics/shaders/sprite.vert", "../Graphics/shaders/sprite.frag", "default");
			ResourceManager::LoadShader("../Graphics/shaders/font.vert", "../Graphics/shaders/font.frag", "text");
            
            s.use();
            s.setInt("image", 0);
            s.setMat4("projection", graphics.projection);
			clearColor = Colors::CornflowerBlue;

			//glEnable(GL_DEPTH_TEST);
		}


		void ProcessInput(std::chrono::microseconds elapsedTime) override
		{
			Ebony::PressedState keyState = keyInput.getIsKeyDown(GLFW_KEY_E);

			if (keyState == PressedState::HELD)
			{
				std::cout << "Held" << std::endl;
			}
			else if (keyState == PressedState::PRESSED)
			{
				std::cout << "Pressed" << std::endl;
			}
			else if (keyState == PressedState::RELEASED)
			{
				std::cout << "Released" << std::endl;
			}

			if (keyInput.getKeyPressedOrHeld(GLFW_KEY_ESCAPE))
			{
				glfwSetWindowShouldClose(graphics.window.getWindow(), true);
			}

		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			fpsUpdateDeltaTime -= deltaTime;


			if (fpsUpdateDeltaTime <= 1.0f)
			{
				fps = std::to_string(static_cast<int>(std::round(1 / deltaTime))) + " fps";
				fpsUpdateDeltaTime += 0.25f;
				layer = (layer + 1) % 44;
				Shader& s = ResourceManager::GetShader("spritesheet");
				s.use();
				s.setInt("layer", layer);

			}

		}


		void Draw(std::chrono::microseconds elapsedTime) override
		{
			
			//std::cout << "Drawing!" << std::endl;
			graphics.BeginDraw(clearColor);


			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glEnable(GL_DEPTH_TEST);

			//graphics.SetRenderTarget(main, Colors::Red);
			graphics.Draw(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 0.0f), glm::vec2(300.0f, 400.0f), 45.0f, Colors::Red);
			graphics.Draw(ResourceManager::GetShader("spritesheet"), ResourceManager::GetTexture("massiveTextureAtlas"), glm::vec2(200.0f, 100.0f), glm::vec2(100.0f, 100.0f), 0.0f, Colors::Red);
			graphics.DrawString(ResourceManager::GetShader("text"), spriteFont, fps, 25.0f, 100.0f, 1.0f, Colors::Red);
			//graphics.UnbindRenderTarget(clearColor);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			//(GL_COLOR_BUFFER_BIT);
			ResourceManager::GetShader("screentexture").use();
			glBindVertexArray(graphics.quadRenderTarget);
			glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//graphics.DrawRenderTarget(ResourceManager::GetShader("screentexture"), main);



			graphics.EndDraw();

		}

		void Run() override
		{
			WindowResizeEvent e(1280, 720);
			EB_TRACE(e);

			Init();
			Shader& s1 = ResourceManager::LoadShader("../Graphics/shaders/screenTexture.vert", "../Graphics/shaders/screenTexture.frag", "screenTexture");

			s1.use();
			s1.setInt("screenTexture", 0);

			graphics.InitializeTextDrawing(ResourceManager::GetShader("text"));
			spriteFont.LoadFont("../Graphics/fonts/super-indie-font/SuperIndie.ttf");
			Shader& s = ResourceManager::LoadShader("../Graphics/shaders/spritesheet3d.vert", "../Graphics/shaders/spritesheet3d.frag", "spritesheet");

			s.use();
			s.setInt("spritesheet", 0);
			s.setMat4("projection", graphics.projection);



			ResourceManager::LoadAtlas("../Graphics/textures/test.tx", "test", 1, 4);
			ResourceManager::LoadAtlas("../Graphics/textures/sampleSpriteSheet.tx", "sampleSpritesheet", 6, 1);
			ResourceManager::LoadAtlas("../Graphics/textures/massiveTextureAtlas.tx", "massiveTextureAtlas", 32, 24); // Actually has 64 x 48 but you can't have that many images in a 3D array. Need to enforce size limits
			ResourceManager::LoadAtlas("../Graphics/textures/Better_Character_Animation.tx", "Better_Character_Animation", 44, 1);

			main = RenderTarget2D::Create(graphics.screenWidth, graphics.screenHeight, GL_LINEAR, GL_NEAREST);

			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			glGenTextures(1, &textureColorBuffer);
			glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, graphics.screenWidth, graphics.screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, graphics.screenWidth, graphics.screenHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			auto previousTime = std::chrono::system_clock::now();
			while (!glfwWindowShouldClose(graphics.window.getWindow()))
			{
				auto currentTime = std::chrono::system_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);

				ProcessInput(elapsedTime);
				Update(elapsedTime);
				Draw(elapsedTime);
				glfwPollEvents();
			}

			glfwTerminate();
		}


	public:
		Graphics2d graphics;
		KeyInput keyInput;
		Color clearColor;
		SpriteFont spriteFont;
		
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		float fpsUpdateDeltaTime = 0.0f;
		int layer = 0;
		std::string fps = "";
		RenderTarget2D main;
		unsigned int framebuffer;
		unsigned int textureColorBuffer;
		unsigned int rbo;

	
	};


	Ebony::Application* Ebony::CreateApplication()
	{
		return new Sandbox();
	}

}