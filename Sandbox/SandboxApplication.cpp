#include "Ebony.hpp"
#include "camera.hpp"


#include "glm/glm.hpp"
#include <resourceManager.hpp>
#include <filesystem>
#include <spritefont.hpp>
#include <systems/particleSystem.hpp>
#include <systems/particleRenderer.hpp>
#include <systems/inputSystem.hpp>


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

			std::vector<int> keys = { GLFW_KEY_E, GLFW_KEY_ESCAPE, GLFW_GAMEPAD_BUTTON_CIRCLE };
			keyInput.setKeysToMonitorInit(keys);

			testParticles = std::make_shared<entities::Entity>();
			keyboardInput = std::make_shared<entities::Entity>();

            Texture2D& faceTexture = ResourceManager::LoadTexture("textures/awesomeface.tx", "face");
            Shader& s = ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
			ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");
            
            s.use();
            s.setInt("image", 0);
            s.setMat4("projection", graphics.projection);
			clearColor = Colors::CornflowerBlue;


			particleSystem = systems::ParticleSystem();
			particleRenderer = systems::ParticleRenderer();

			auto particleGroup = std::make_unique<components::ParticleGroup>(ResourceManager::GetTexture("face"), 100000);
			particleGroup->velocity = glm::vec2{ 50.0f, 10.0f };
			particleGroup->rateOverTime = 1500;
			particleGroup->spawnRate = std::chrono::milliseconds(16);
			particleGroup->position = glm::vec2{ 400.0f, 400.0f };
			particleGroup->maxLifetime = std::chrono::milliseconds(500);
			particleGroup->startSize = glm::vec2{ 1.0f, 1.0f };
			particleGroup->endSize = glm::vec2{ 1.0f, 1.0f };
			particleGroup->startAlpha = 1.0f;
			particleGroup->endAlpha = 1.0f;
			particleGroup->startColor = Ebony::Colors::White;
			particleGroup->endColor = Ebony::Colors::White;
			//particleGroup->maxDuration = std::chrono::seconds(3);
			//particleGroup->startDelay = std::chrono::seconds(5);

			testParticles->addComponent(std::move(particleGroup));

			particleSystem.AddEntity(testParticles);
			particleRenderer.AddEntity(testParticles);


			inputSystem = systems::InputSystem();

			// TODO: Right now, it just defaults to be the first controller. Add support for multiple controllers in the future
			std::unique_ptr<components::Input> inputComponent = std::make_unique<components::Input>(0);
			
			inputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_START, [=]() {glfwSetWindowShouldClose(graphics.window.getWindow(), true); } });
			inputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_CIRCLE, [=]() { std::cout << "Circle was called" << std::endl; } });
			inputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_CROSS, [=]() { std::cout << "Cross was called" << std::endl; } });
			inputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_SQUARE, [=]() { std::cout << "Square was called" << std::endl; } });
			inputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_TRIANGLE, [=]() { std::cout << "Triangle was called" << std::endl; } });
			inputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_LEFT_X, [=](float value) { 
				if (abs(value) > 0.5)
				{
					std::cout << "Left X: " << value << std::endl;
				}
				}});

			inputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_LEFT_Y, [=](float value) {
				if (abs(value) > 0.5)
				{
					std::cout << "Left Y: " << value << std::endl;
				}
				} });

			inputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_RIGHT_X, [=](float value) {
				if (abs(value) > 0.5)
				{
					std::cout << "Right X: " << value << std::endl;
				}
				} });

			inputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_RIGHT_Y, [=](float value) {
				if (abs(value) > 0.5)
				{
					std::cout << "Right Y: " << value << std::endl;
				}
				} });

			inputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, [=](float value) {
				if (value > -0.5)
				{
					std::cout << "Left Trigger: " << value << std::endl;
				}
			} });

			inputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, [=](float value) {
			if (value > -0.5)
			{
				std::cout << "Right Trigger: " << value << std::endl;
			}
			} });


			inputComponent->keyboardActionKeyPairs.insert({ GLFW_KEY_ESCAPE, [=]() {glfwSetWindowShouldClose(graphics.window.getWindow(), true); } });
			inputComponent->keyboardActionKeyPairs.insert({ GLFW_KEY_E, [=]() { std::cout << "E was called" << std::endl; } });


			keyboardInput->addComponent(std::move(inputComponent));


			inputSystem.AddEntity(keyboardInput);
		}


		void ProcessInput(std::chrono::microseconds elapsedTime) override
		{
			inputSystem.Update(keyInput);
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			fpsUpdateDeltaTime -= deltaTime;


			auto previousTime = std::chrono::system_clock::now();
			particleSystem.Update(elapsedTime);
			averageParticleSystemTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);

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
			
			auto previousTime = std::chrono::system_clock::now();
			particleRenderer.Update(graphics);
			averageParticleRenderingTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);


			//graphics.Draw(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 0.0f), glm::vec2(300.0f, 400.0f), 45.0f, Colors::Red, 0.0f);
			graphics.Draw(ResourceManager::GetShader("spritesheet"), ResourceManager::GetTexture("massiveTextureAtlas"), glm::vec2(200.0f, 100.0f), glm::vec2(100.0f, 100.0f), 0.0f, Colors::Red, 1.0f);
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
				totalFrames++;
			}

			std::cout << "Particle Rendering took " << averageParticleRenderingTime / totalFrames << " on average." << std::endl;
			std::cout << "Particle System Updates took " << averageParticleSystemTime / totalFrames << " on average." << std::endl;
			std::cout << "Particle Count at termination: " << testParticles->getComponent<components::ParticleGroup>()->particleCount << std::endl;
			glfwTerminate();
		}


	public:
		Graphics2d graphics;
		KeyInput keyInput;
		Color clearColor;
		SpriteFont spriteFont;
		systems::ParticleSystem particleSystem;
		systems::ParticleRenderer particleRenderer;
		systems::InputSystem inputSystem;

		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		float fpsUpdateDeltaTime = 0.0f;
		std::chrono::microseconds averageParticleRenderingTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageParticleSystemTime = std::chrono::microseconds::zero();
		std::uint64_t totalFrames = 0;


		int layer = 0;
		std::string fps = "";
		RenderTarget2D main;
		entities::EntityPtr testParticles;
		entities::EntityPtr keyboardInput;

	
	};


	Ebony::Application* Ebony::CreateApplication()
	{
		return new Sandbox();
	}

}