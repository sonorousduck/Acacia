#include "Ebony.hpp"
#include "camera.hpp"


#include "glm/glm.hpp"
#include <resourceManager.hpp>
#include <filesystem>
#include <spritefont.hpp>
#include <systems/particleSystem.hpp>
#include <systems/particleRenderer.hpp>



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
			Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

			graphics.Initialize("Ebony", 800, 600);
			graphics.SetMainCamera(camera);
			Ebony::KeyInput::setupKeyInputs(graphics.window);

			std::vector<int> keys = { GLFW_KEY_E, GLFW_KEY_ESCAPE };
			keyInput.setKeysToMonitorInit(keys);

			testParticles = std::make_shared<entities::Entity>();

            Texture2D& faceTexture = ResourceManager::LoadTexture("textures/awesomeface.tx", "face");
            Shader& s = ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
			ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");
            
            s.use();
            s.setInt("image", 0);
            s.setMat4("projection", graphics.projection);
			clearColor = Colors::CornflowerBlue;


			

			particleSystem = systems::ParticleSystem();
			particleRenderer = systems::ParticleRenderer();

			auto particleGroup = std::make_unique<components::ParticleGroup>(ResourceManager::GetTexture("face"));
			particleGroup->velocity = glm::vec2{ 0.5f, 0.5f };
			particleGroup->rateOverTime = 5;

			testParticles->addComponent(std::move(particleGroup));

			particleSystem.AddEntity(testParticles);
			particleRenderer.AddEntity(testParticles);
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

			particleSystem.Update(elapsedTime);

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
			graphics.BeginDraw(clearColor);

			graphics.SetRenderTarget(main, clearColor);
			
			particleRenderer.Update(graphics);

			//graphics.Draw(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 0.0f), glm::vec2(300.0f, 400.0f), 45.0f, Colors::Red, 0.0f);
			//graphics.Draw(ResourceManager::GetShader("spritesheet"), ResourceManager::GetTexture("massiveTextureAtlas"), glm::vec2(200.0f, 100.0f), glm::vec2(100.0f, 100.0f), 0.0f, Colors::Red, 1.0f);
			graphics.DrawString(ResourceManager::GetShader("text"), spriteFont, fps, 25.0f, 100.0f, 1.0f, Colors::Red);
			
			graphics.UnbindRenderTarget(clearColor);

			graphics.DrawRenderTarget(ResourceManager::GetShader("screenTexture"), main);



			graphics.EndDraw();
		}

		void Run() override
		{
			WindowResizeEvent e(1280, 720);
			EB_TRACE(e);

			Init();
			Shader& s1 = ResourceManager::LoadShader("shaders/screenTexture.vert", "shaders/screenTexture.frag", "screenTexture");

			s1.use();
			s1.setInt("screenTexture", 0);

			graphics.InitializeTextDrawing(ResourceManager::GetShader("text"));
			spriteFont.LoadFont("fonts/super-indie-font/SuperIndie.ttf");
			Shader& s = ResourceManager::LoadShader("shaders/spritesheet3d.vert", "shaders/spritesheet3d.frag", "spritesheet");

			s.use();
			s.setInt("spritesheet", 0);
			s.setMat4("projection", graphics.projection);

			Shader& s2 = ResourceManager::LoadShader("shaders/particle.vert", "shaders/particle.frag", "defaultParticle");

			s2.use();
			s2.setInt("particleTexture", 0);
			s2.setMat4("projection", graphics.projection);

			ResourceManager::LoadAtlas("textures/test.tx", "test", 1, 4);
			ResourceManager::LoadAtlas("textures/sampleSpriteSheet.tx", "sampleSpritesheet", 6, 1);
			ResourceManager::LoadAtlas("textures/massiveTextureAtlas.tx", "massiveTextureAtlas", 32, 24); // Actually has 64 x 48 but you can't have that many images in a 3D array. Need to enforce size limits
			ResourceManager::LoadAtlas("textures/Better_Character_Animation.tx", "Better_Character_Animation", 44, 1);

			main = RenderTarget2D::Create(graphics.screenWidth, graphics.screenHeight, GL_LINEAR, GL_NEAREST);


			auto previousTime = std::chrono::system_clock::now();

			while (!glfwWindowShouldClose(graphics.window.getWindow()))
			{
				auto currentTime = std::chrono::system_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
				previousTime = currentTime;

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
		systems::ParticleSystem particleSystem;
		systems::ParticleRenderer particleRenderer;

		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		float fpsUpdateDeltaTime = 0.0f;
		int layer = 0;
		std::string fps = "";
		RenderTarget2D main;
		entities::EntityPtr testParticles;

	
	};


	Ebony::Application* Ebony::CreateApplication()
	{
		return new Sandbox();
	}

}