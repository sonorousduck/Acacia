#include "Ebony.hpp"
#include "camera.hpp"
#include "glm/glm.hpp"
#include "misc/resourceManager.hpp"
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

		void LoadContent()
		{
			//std::atomic_bool success{ true };
			//std::latch contentLoaded{ 1 };

			//auto onComplete = [&]([[maybe_unused]] std::string key)
			//{
			//	contentLoaded.count_down();
			//};
			// TODO: Right now, there is no indicator whether it was successful or not
			/*auto onError = [&]([[maybe_unused]] std::string key)
			{
				success = false;
				contentLoaded.count_down();
			};*/

			//ResourceManager::LoadTextureAsync("textures/awesomeface.tx", "face", onComplete);
			//ResourceManager::LoadSoundEffect("SoundEffects/wall.wav", "wall");
			//ResourceManager::LoadSoundEffect("SoundEffects/magnet_action.wav", "magnet_action");
			//ResourceManager::LoadAtlasAsync("textures/test.tx", "test", 1, 4, nullptr);
			//ResourceManager::LoadAtlasAsync("textures/sampleSpriteSheet.tx", "sampleSpritesheet", 6, 1, nullptr);
			//ResourceManager::LoadAtlasAsync("textures/massiveTextureAtlas.tx", "massiveTextureAtlas", 32, 24, onComplete); // Actually has 64 x 48 but you can't have that many images in a 3D array. Need to enforce size limits

			ResourceManager::LoadAtlas("textures/test.tx", "test", 1, 4);
			ResourceManager::LoadAtlas("textures/sampleSpriteSheet.tx", "sampleSpritesheet", 6, 1);
			ResourceManager::LoadAtlas("textures/massiveTextureAtlas.tx", "massiveTextureAtlas", 32, 24); // Actually has 64 x 48 but you can't have that many images in a 3D array. Need to enforce size limits

			//contentLoaded.wait();

		}


		void Init() override
		{
			Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

			graphics.Initialize("Ebony", 800, 600);
			SoundDevice::init();
			EbonyAudio::Music::Init();
			LoadContent();


			graphics.SetMainCamera(camera);
			Ebony::KeyInput::setupKeyInputs(graphics.window);
			Ebony::MouseInput::setupMouseInputs(graphics.window);


			std::vector<int> keys = { GLFW_KEY_E, GLFW_KEY_ESCAPE, GLFW_KEY_LEFT_SHIFT };
			keyInput.setKeysToMonitorInit(keys);

			// All buttons defined here: https://www.glfw.org/docs/3.3/group__buttons.html
			std::vector<int> buttons = { GLFW_MOUSE_BUTTON_1 };
			mouseInput.setButtonsToMonitor(buttons);

			std::vector<int> controllerButtons = { GLFW_GAMEPAD_BUTTON_CROSS, GLFW_GAMEPAD_BUTTON_CIRCLE, GLFW_GAMEPAD_BUTTON_TRIANGLE, GLFW_GAMEPAD_BUTTON_SQUARE, GLFW_GAMEPAD_BUTTON_START };
			controllerInput.setButtonsToMonitorInit(controllerButtons);



			testParticles = std::make_shared<entities::Entity>();
			keyboardInput = std::make_shared<entities::Entity>();
			animationsTest = std::make_shared<entities::Entity>();
			testEntity = std::make_shared<entities::Entity>();

			particleSystem = systems::ParticleSystem();
			particleRenderer = systems::ParticleRenderer();
			animationRenderer = systems::AnimationRenderer();
			animationSystem = systems::Animation2d();
			fontRenderer = systems::FontRenderer();
			audioSystem = systems::AudioSystem();


			//mySpeaker = SoundSource();



            Shader& s = ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
			ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");

			//ResourceManager::LoadMusic("Music/song18.mp3", "song18");
			//ResourceManager::LoadMusic("Music/TownTheme.wav", "TownTheme");

			ResourceManager::LoadSoundEffect("SoundEffects/wall.wav", "wall");
			ResourceManager::LoadTexture("textures/awesomeface.tx", "face");
			ResourceManager::LoadAtlas("textures/Better_Character_Animation.tx", "Better_Character_Animation", 44, 1);



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
			

			auto textComponent = std::make_unique<components::Text>(fps, glm::vec2{25.0f, 100.0f}, 1.0f, Ebony::Colors::Black, Ebony::Colors::White, spriteFont);
			testEntity->addComponent(std::move(textComponent));
			
			AddEntity(testEntity);


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

			AddEntity(testParticles);



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

				AddEntity(test);
			}
			

			inputSystem = systems::InputSystem();

			// TODO: Right now, it just defaults to be the first controller. Add support for multiple controllers in the future
			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();

			//controllerInputComponent->bindings.insert({ GLFW_GAMEPAD_BUTTON_START, "quit" });
			//controllerInputComponent->bindings.insert({ GLFW_GAMEPAD_BUTTON_CIRCLE, "print" });
			//controllerInputComponent->bindings.insert({ GLFW_GAMEPAD_BUTTON_TRIANGLE, "printRelease" });



			controllerInputComponent->onPressActions.insert({ "quit", [=]() {glfwSetWindowShouldClose(graphics.window.getWindow(), true); } });
			controllerInputComponent->onHeldActions.insert({ "print", [=]() {std::cout << "Circle was called (OnHeld)" << std::endl; } });
			controllerInputComponent->onReleaseActions.insert({ "printRelease", [=]() {std::cout << "Triangle was called" << std::endl; } });

			/*controllerInputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_START, [=]() {glfwSetWindowShouldClose(graphics.window.getWindow(), true); } });
			controllerInputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_CIRCLE, [=]() { std::cout << "Circle was called" << std::endl; } });
			controllerInputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_CROSS, [=]() { std::cout << "Cross was called" << std::endl; } });
			controllerInputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_SQUARE, [=]() { std::cout << "Square was called" << std::endl; } });
			controllerInputComponent->controllerActionKeyPairs.insert({ GLFW_GAMEPAD_BUTTON_TRIANGLE, [=]() { std::cout << "Triangle was called" << std::endl; } });*/

			//controllerInputComponent->joystickBindings.insert({ GLFW_GAMEPAD_AXIS_LEFT_X, "left" });
			//controllerInputComponent->joystickBindings.insert({ GLFW_GAMEPAD_AXIS_LEFT_Y, "right" });
			//controllerInputComponent->joystickBindings.insert({ GLFW_GAMEPAD_AXIS_RIGHT_X, "up" });
			//controllerInputComponent->joystickBindings.insert({ GLFW_GAMEPAD_AXIS_RIGHT_Y, "down" });
			//controllerInputComponent->joystickBindings.insert({ GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, "shoot" });

			controllerInputComponent->joystickActions.insert({ "left", [=](float value) {
				if (abs(value) > 0.10)
				{
					std::cout << "Left X: " << value << std::endl;
				}
				} });

			controllerInputComponent->joystickActions.insert({ "right", [=](float value) {
				if (abs(value) > 0.10)
				{
					std::cout << "Left Y: " << value << std::endl;
				}
			} });
			
			controllerInputComponent->joystickActions.insert({ "up", [=](float value) {
				if (abs(value) > 0.10)
				{
					std::cout << "Right X: " << value << std::endl;
				}
				} });
			
			controllerInputComponent->joystickActions.insert({ "down", [=](float value) {
				if (abs(value) > 0.10)
				{
					std::cout << "Right Y: " << value << std::endl;
				}
				} });

			controllerInputComponent->joystickActions.insert({ "shoot", [=](float value) {
				if (value > -0.25)
				{
					std::cout << "Right Trigger: " << value << std::endl;
				}
				} });

			controllerInputComponent->loadControllerBindings("../controllerBindings.json", "../joystickBindings.json");

			//controllerInputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, [=](float value) {
			//if (value > -0.5)
			//{
			//	std::cout << "Right Trigger: " << value << std::endl;
			//}
			//} });


			//keyboardInputComponent->bindings.insert({ GLFW_KEY_ESCAPE, "quit" });
			keyboardInputComponent->onPressActions.insert({ "quit", [=]() {glfwSetWindowShouldClose(graphics.window.getWindow(), true); } });

			//keyboardInputComponent->bindings.insert({ GLFW_KEY_E, "print" });
			keyboardInputComponent->onReleaseActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->onHeldActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });

			//keyboardInputComponent->bindings.insert({ GLFW_KEY_LEFT_SHIFT, "toggleRun" });
			keyboardInputComponent->onReleaseActions.insert({ "toggleRun", [=]() { isRunning = false; } });
			keyboardInputComponent->onPressActions.insert({ "toggleRun", [=]() { isRunning = true; } });


			keyboardInputComponent->loadKeyBindings("../keyBindings.json");


			keyboardInput->addComponent(std::move(controllerInputComponent));
			keyboardInput->addComponent(std::move(keyboardInputComponent));

			AddEntity(keyboardInput);
			AddEntity(testEntity);

			entities::EntityPtr anotherEntity = std::make_shared<entities::Entity>();
			auto mouseComponent = std::make_unique<components::MouseInput>();

			//mouseComponent->bindings.insert({ GLFW_MOUSE_BUTTON_1, "mousePress" });
			mouseComponent->onPressActions.insert({ "mousePress", [=]() {std::cout << "Button pressed!" << std::endl; }});
			mouseComponent->onReleaseActions.insert({ "mousePress",[=]() {std::cout << "Mouse Button released!" << std::endl; }});
			
			mouseComponent->loadMouseBindings("../mouseBindings.json");

			anotherEntity->addComponent(std::move(mouseComponent));

			//keyboardInput->getComponent<components::KeyboardInput>()->saveKeyBindings("../keyBindings.json");
			//keyboardInput->getComponent<components::ControllerInput>()->saveControllerBindings("../controllerBindings.json", "../joystickBindings.json");

			AddEntity(anotherEntity);


			AddNewEntities();
			EB_TRACE("Added all entities");
		}


		void ProcessInput(std::chrono::microseconds elapsedTime) override
		{
			inputSystem.Update(keyInput, mouseInput, controllerInput);
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
			graphics.BeginImgui();

			graphics.SetRenderTarget(main, clearColor);
			
			animationRenderer.Update(graphics);

			auto previousTime = std::chrono::system_clock::now();
			particleRenderer.Update(graphics);
			averageParticleRenderingTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);
			
			fontRenderer.Update(graphics);
			
			graphics.UnbindRenderTarget(clearColor);

			graphics.DrawRenderTarget(ResourceManager::GetShader("screenTexture"), main);


			
			graphics.DrawWindow("Ebony");
			graphics.ImguiText("Hello there!");
			graphics.ImguiText("General Kenobi");

			ImGui::ArrowButton("Left", ImGuiDir_Left);
			
			graphics.CompleteWindow();


			graphics.EndImgui();
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



			main = RenderTarget2D::Create(graphics.screenWidth, graphics.screenHeight, GL_LINEAR, GL_NEAREST);


			auto previousTime = std::chrono::system_clock::now();

			while (!glfwWindowShouldClose(graphics.window.getWindow()))
			{
				auto currentTime = std::chrono::system_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
				previousTime = currentTime;


				// Was hoping this would help the scaling issues, but it does not
				//int bufferWidth = 0;
				//int bufferHeight = 0;
				//glfwGetFramebufferSize(graphics.window.getWindow(), &bufferWidth, &bufferHeight);
				//glViewport(0, 0, bufferWidth, bufferHeight);

				ProcessInput(elapsedTime);
				Update(elapsedTime);
				Draw(elapsedTime);
				glfwPollEvents();
				totalFrames++;
			}

			EB_INFO("Particle Rendering took " + std::to_string(averageParticleRenderingTime.count() / totalFrames) + "us on average.");
			EB_INFO("Average Updates took " + std::to_string(averageUpdateTime.count() / totalFrames) + "us on average.");
			EB_INFO("Particle Count at termination: " + std::to_string(testParticles->getComponent<components::ParticleGroup>()->particleCount));

			glfwTerminate();
			ThreadPool::terminate();
		}

		void AddEntity(entities::EntityPtr entity)
		{
			std::lock_guard<std::mutex> lock(mutexEntities);

			newEntities.push_back(entity);
		}

		void RemoveEntity(entities::Entity::IdType id)
		{
			std::lock_guard<std::mutex> lock(mutexEntities);

			removeEntities.insert(id);
		}
		
		void AddNewEntities() override
		{
			for (auto&& entity : newEntities)
			{
				// Add all systems here
				particleSystem.AddEntity(entity);
				particleRenderer.AddEntity(entity);
				animationRenderer.AddEntity(entity);
				animationSystem.AddEntity(entity);
				inputSystem.AddEntity(entity);
				audioSystem.AddEntity(entity);
				fontRenderer.AddEntity(entity);


				allEntities[entity->getId()] = entity;
			}

			newEntities.clear();
		}

		void RemoveOldEntities() override
		{
			for (auto&& entityId : removeEntities)
			{
				allEntities.erase(entityId);

				particleSystem.RemoveEntity(entityId);
				particleRenderer.RemoveEntity(entityId);
				animationRenderer.RemoveEntity(entityId);
				animationSystem.RemoveEntity(entityId);
				inputSystem.RemoveEntity(entityId);
				audioSystem.RemoveEntity(entityId);
				fontRenderer.RemoveEntity(entityId);
			}
		}



	public:
		Graphics2d graphics;
		KeyInput keyInput;
		MouseInput mouseInput;
		ControllerInput controllerInput;
		Color clearColor;
		SpriteFont spriteFont;
		systems::ParticleSystem particleSystem;
		systems::ParticleRenderer particleRenderer;
		systems::AnimationRenderer animationRenderer;
		systems::Animation2d animationSystem;
		systems::InputSystem inputSystem;
		systems::AudioSystem audioSystem;
		systems::FontRenderer fontRenderer;

		entities::EntityMap allEntities;
		std::vector<entities::EntityPtr> newEntities;
		std::unordered_set<entities::Entity::IdType> removeEntities;
		std::unordered_set<entities::Entity::IdType> updatedEntities;
		std::mutex mutexEntities; // This is necessary so we can add to the newEntities vector even though it is multithreaded

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