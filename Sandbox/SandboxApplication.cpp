#include "Ebony.hpp"
#include "camera.hpp"
#include "glm/glm.hpp"
#include "resourceManager.hpp"
#include "spritefont.hpp"
#include "systems/particleSystem.hpp"
#include "systems/particleRenderer.hpp"
#include "systems/inputSystem.hpp"
#include "systems/animationRenderer.hpp"
#include "systems/animation2d.hpp"
#include "misc/ThreadPool.hpp"

#include <filesystem>
#include <latch>
#include <algorithm>
#include <systems/audioSystem.hpp>
#include "../Audio/src/SoundBuffer.hpp"
#include <components/text.hpp>
#include <systems/fontRenderer.hpp>
#include <components/mouseInputComponent.hpp>


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
			SoundDevice::init();
			EbonyAudio::Music::Init();
			graphics.SetMainCamera(camera);
			Ebony::KeyInput::setupKeyInputs(graphics.window);



			std::vector<int> keys = { GLFW_KEY_E, GLFW_KEY_ESCAPE, GLFW_KEY_LEFT_SHIFT };
			keyInput.setKeysToMonitorInit(keys);

			testParticles = std::make_shared<entities::Entity>();
			keyboardInput = std::make_shared<entities::Entity>();
			animationsTest = std::make_shared<entities::Entity>();
			testEntity = std::make_shared<entities::Entity>();

			particleSystem = systems::ParticleSystem();
			particleRenderer = systems::ParticleRenderer();
			animationRenderer = systems::AnimationRenderer();
			animationSystem = systems::Animation2d();
			fontRenderer = systems::FontRenderer();
			//audioSystem = systems::AudioSystem();


			//mySpeaker = SoundSource();



            Texture2D& faceTexture = ResourceManager::LoadTexture("textures/awesomeface.tx", "face");
            Shader& s = ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
			ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");

			//ResourceManager::LoadMusic("Music/song18.mp3", "song18");
			//ResourceManager::LoadMusic("Music/TownTheme.wav", "TownTheme");

			//ResourceManager::LoadSoundEffect("SoundEffects/wall.wav", "wall");

			ResourceManager::LoadSoundEffect("SoundEffects/wall.wav", "wall");
			ResourceManager::LoadSoundEffect("SoundEffects/magnet_action.wav", "magnet_action");


			graphics.InitializeTextDrawing(ResourceManager::GetShader("text"));
			spriteFont.LoadFont("fonts/super-indie-font/SuperIndie.ttf");


            s.use();
            s.setInt("image", 0);
            s.setMat4("projection", graphics.projection);
			clearColor = Colors::CornflowerBlue;

			//SoundSource mySpeaker{};
			//mySpeaker.Play(ResourceManager::GetSoundEffect("magnet_action"));



			//auto audioComponent = std::make_unique<components::AudioSource>();
			//audioComponent->soundEffectQueue.push_back(ResourceManager::GetSoundEffect("magnet_action"));
			//testEntity->addComponent(std::move(audioComponent));

			//audioSystem.AddEntity(testEntity);

			auto textComponent = std::make_unique<components::Text>(fps, glm::vec2{25.0f, 100.0f}, 1.0f, Ebony::Colors::Black, Ebony::Colors::White, spriteFont);
			testEntity->addComponent(std::move(textComponent));
			fontRenderer.AddEntity(testEntity);


			auto particleGroup = components::ParticleGroup::Cone(ResourceManager::GetTexture("face"), glm::vec2(10.0f, -10.0f), 45.0f, 1000);
			//auto particleGroup = std::make_unique<components::ParticleGroup>(ResourceManager::GetTexture("face"), 100000);
			//particleGroup->velocity = glm::vec2{ 50.0f, 10.0f };
			particleGroup->rateOverTime = 1;
			particleGroup->spawnRate = std::chrono::milliseconds(16);
			particleGroup->position = glm::vec2{ 400.0f, 400.0f };
			particleGroup->maxLifetime = std::chrono::milliseconds(500);
			particleGroup->startSize = glm::vec2{ 1.0f, 1.0f };
			particleGroup->maxStartSize = glm::vec2{ 1.0f, 1.0f };
			//particleGroup->randomStartSize = true;

			//particleGroup->startSpeed = glm::vec2{ 0.0f, -10.0f };
			//particleGroup->maxStartSpeed = glm::vec2{ 50.0f, 10.0f };
			//particleGroup->randomStartSpeed = true;

			particleGroup->endSize = glm::vec2{ 1.0f, 1.0f };
			particleGroup->startAlpha = 1.0f;
			particleGroup->endAlpha = 1.0f;
			particleGroup->startColor = Ebony::Colors::White;
			particleGroup->endColor = Ebony::Colors::White;
			//particleGroup->emissionArea = glm::vec2(100.0f, 100.0f);
			//particleGroup->maxDuration = std::chrono::seconds(3);
			//particleGroup->startDelay = std::chrono::seconds(5);

			testParticles->addComponent(std::move(particleGroup));

			particleSystem.AddEntity(testParticles);
			particleRenderer.AddEntity(testParticles);



			for (size_t i = 0; i < 1; i++)
			{
				entities::EntityPtr test = std::make_shared<entities::Entity>();;


				auto animationController = std::make_unique<components::AnimationController>();


				std::vector<std::chrono::microseconds> timings1(44, std::chrono::milliseconds(500 + 1000 - i));
				std::vector<std::chrono::microseconds> timings2(44, std::chrono::milliseconds(100 + 1000 - i));

				std::vector<components::Link> links1 = { components::Link(1, [=]() {
					return (isRunning);

					}) };
				std::vector<Ebony::Animation> animations1 = { Ebony::Animation(SpriteSheet(ResourceManager::GetTexture("Better_Character_Animation"), 44, timings1)) };
				animations1[0].SetDepth((1.0f / 1000) * i);

				auto node1 = components::Node(links1, animations1);

				std::vector<components::Link> links2 = { components::Link(0, [=]() { return (!isRunning); }) };
				std::vector<Ebony::Animation> animations2 = { Ebony::Animation(SpriteSheet(ResourceManager::GetTexture("Better_Character_Animation"), 44, timings2)) };
				animations2[0].SetDepth((1.0f / 1000) * i);

				auto node2 = components::Node(links2, animations2);


				animationController->animationTree.emplace_back(node1);
				animationController->animationTree.emplace_back(node2);


				test->addComponent(std::move(animationController));

				animationSystem.AddEntity(test);
				animationRenderer.AddEntity(test);
			}
			



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
			inputComponent->keyboardActionKeyPairs.insert({ GLFW_KEY_LEFT_SHIFT, [=]() { isRunning = true; } });
			inputComponent->onReleaseKeyboardActionKeyPairs.insert({ GLFW_KEY_LEFT_SHIFT, [=]() { isRunning = false; } });

			auto mouseComponent = std::make_unique<components::MouseInput>();



			keyboardInput->addComponent(std::move(inputComponent));
			testEntity->addComponent(std::move(mouseComponent));

			inputSystem.AddEntity(keyboardInput);
			inputSystem.AddEntity(testEntity);
		}


		void ProcessInput(std::chrono::microseconds elapsedTime) override
		{
			inputSystem.Update(keyInput);
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			auto firstTime = std::chrono::system_clock::now();
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			fpsUpdateDeltaTime -= deltaTime;

			std::latch graphDone{ 1 };

			auto taskGraph = ThreadPool::instance().createTaskGraph(
				[&graphDone]()
				{
					graphDone.count_down();
				});

			auto task1 = ThreadPool::instance().createTask(
				taskGraph,
				[this, elapsedTime]()
				{
					animationSystem.Update(elapsedTime);
				}
			);

			auto task2 = ThreadPool::instance().createTask(
				taskGraph,
				[this, elapsedTime]()
				{
					particleSystem.Update(elapsedTime);
				}
			);

			//auto task3 = ThreadPool::instance().createTask(
			//	taskGraph,
			//	[this, elapsedTime]()
			//	{
			//	}
			//);

			// Declare predecessors here
			//taskGraph->declarePredecessor(task1->getId(), task2->getId());


			ThreadPool::instance().submitTaskGraph(taskGraph);
			graphDone.wait();

			//audioSystem.Update(elapsedTime);

			/*auto previousTime = std::chrono::system_clock::now();
			animationSystem.Update(elapsedTime);
			averageAnimationSystemTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);

			previousTime = std::chrono::system_clock::now();
			particleSystem.Update(elapsedTime);
			averageParticleSystemTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);*/

			if (fpsUpdateDeltaTime <= 1.0f)
			{
				fps = std::to_string(static_cast<int>(std::round(1 / deltaTime))) + " fps";
				fpsUpdateDeltaTime += 0.16f;
				testEntity->getComponent<components::Text>()->text = fps;
			}

			averageUpdateTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - firstTime);
		}


		void Draw(std::chrono::microseconds elapsedTime) override
		{
			graphics.BeginDraw(clearColor);

			graphics.SetRenderTarget(main, clearColor);
			
			animationRenderer.Update(graphics);

			auto previousTime = std::chrono::system_clock::now();
			particleRenderer.Update(graphics);
			averageParticleRenderingTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);
			
			fontRenderer.Update(graphics);

			//graphics.DrawString(ResourceManager::GetShader("text"), spriteFont, fps, 25.0f, 100.0f, 1.0f, Colors::Red, Colors::Black);
			
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
			//std::cout << "Particle System Updates took " << averageParticleSystemTime / totalFrames << " on average." << std::endl;
			//std::cout << "Animation System Updates took " << averageAnimationSystemTime / totalFrames << " on average." << std::endl;
			std::cout << "Average Updates took " << averageUpdateTime / totalFrames << " on average." << std::endl;

			std::cout << "Particle Count at termination: " << testParticles->getComponent<components::ParticleGroup>()->particleCount << std::endl;
			glfwTerminate();
			ThreadPool::terminate();
		}


	public:
		Graphics2d graphics;
		KeyInput keyInput;
		Color clearColor;
		SpriteFont spriteFont;
		systems::ParticleSystem particleSystem;
		systems::ParticleRenderer particleRenderer;
		systems::AnimationRenderer animationRenderer;
		systems::Animation2d animationSystem;
		systems::InputSystem inputSystem;
		systems::AudioSystem audioSystem;
		systems::FontRenderer fontRenderer;


		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		float fpsUpdateDeltaTime = 0.0f;
		bool isRunning = false;
		std::chrono::microseconds averageParticleRenderingTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageParticleSystemTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageAnimationSystemTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageUpdateTime = std::chrono::microseconds::zero();

		std::uint64_t totalFrames = 0;


		std::string fps = "";
		RenderTarget2D main;
		entities::EntityPtr testParticles;
		entities::EntityPtr keyboardInput;
		entities::EntityPtr animationsTest;
		entities::EntityPtr testEntity;

	
	};


	Ebony::Application* Ebony::CreateApplication()
	{
		return new Sandbox();
	}

}