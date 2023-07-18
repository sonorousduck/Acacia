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

			//graphics.Draw(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 0.0f), glm::vec2(300.0f, 400.0f), 45.0f, Colors::Red);
			graphics.Draw(ResourceManager::GetShader("default"), ResourceManager::GetTexture("face"), glm::vec2(200.0f, 100.0f), glm::vec2(300.0f, 400.0f), 45.0f, Colors::Red);

			graphics.SetRenderTarget(main, clearColor);
			graphics.Draw(ResourceManager::GetShader("spritesheet"), ResourceManager::GetTexture("massiveTextureAtlas"), glm::vec2(200.0f, 100.0f), glm::vec2(100.0f, 100.0f), 0.0f, Colors::Red);
			graphics.DrawString(ResourceManager::GetShader("text"), spriteFont, fps, 25.0f, 100.0f, 1.0f, Colors::Red);
			graphics.UnbindRenderTarget(clearColor);

			graphics.DrawRenderTarget(ResourceManager::GetShader("screentexture"), main);



			graphics.EndDraw();

		}

		void Run() override
		{
			WindowResizeEvent e(1280, 720);
			EB_TRACE(e);

			Init();
			graphics.InitializeTextDrawing(ResourceManager::GetShader("text"));
			spriteFont.LoadFont("../Graphics/fonts/super-indie-font/SuperIndie.ttf");
			Shader& s = ResourceManager::LoadShader("../Graphics/shaders/spritesheet3d.vert", "../Graphics/shaders/spritesheet3d.frag", "spritesheet");

			s.use();
			s.setInt("spritesheet", 0);
			s.setMat4("projection", graphics.projection);

			Shader& s1 = ResourceManager::LoadShader("../Graphics/shaders/screenTexture.vert", "../Graphics/shaders/screenTexture.frag", "screenTexture");

			s1.use();
			s1.setInt("screenTexture", 0);

			ResourceManager::LoadAtlas("../Graphics/textures/test.tx", "test", 1, 4);
			ResourceManager::LoadAtlas("../Graphics/textures/sampleSpriteSheet.tx", "sampleSpritesheet", 6, 1);
			ResourceManager::LoadAtlas("../Graphics/textures/massiveTextureAtlas.tx", "massiveTextureAtlas", 32, 24); // Actually has 64 x 48 but you can't have that many images in a 3D array. Need to enforce size limits
			ResourceManager::LoadAtlas("../Graphics/textures/Better_Character_Animation.tx", "Better_Character_Animation", 44, 1);

			main = RenderTarget2D::Create(graphics.screenWidth, graphics.screenHeight, GL_LINEAR, GL_NEAREST);

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

	
	};


	Ebony::Application* Ebony::CreateApplication()
	{
		return new Sandbox();
	}

}