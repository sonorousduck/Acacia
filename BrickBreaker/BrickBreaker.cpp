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
#include "../Audio/src/audioManager.hpp"
#include <components/text.hpp>
#include <systems/fontRenderer.hpp>
#include <components/mouseInputComponent.hpp>
#include <systems/physicsSystem.hpp>
#include <systems/spriteRenderer.hpp>
#include <singletons/time.hpp>


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

			ResourceManager::LoadSoundEffect("wall", "../Audio/SoundEffects/wall.wav");
			ResourceManager::LoadMusic("song18", "../Audio/Music/song18.wav");
			ResourceManager::LoadTexture("textures/awesomeface.tx", "face", false);
			ResourceManager::LoadTexture("blue_tile.tx", "blue_tile");
			ResourceManager::LoadTexture("blue_broken_tile.tx", "blue_broken_tile");
			ResourceManager::LoadTexture("grey_tile.tx", "grey_tile");
			ResourceManager::LoadTexture("broken_grey_tile.tx", "grey_broken_tile");
			ResourceManager::LoadTexture("emerald_tile.tx", "emerald_tile");
			ResourceManager::LoadTexture("emerald_broken_tile.tx", "emerald_broken_tile");
			ResourceManager::LoadTexture("green_tile.tx", "green_tile");
			ResourceManager::LoadTexture("green_broken_tile.tx", "green_broken_tile");
			ResourceManager::LoadTexture("yellow_tile.tx", "yellow_tile");
			ResourceManager::LoadTexture("yellow_broken_tile.tx", "yellow_broken_tile");
			ResourceManager::LoadTexture("red_tile.tx", "red_tile");
			ResourceManager::LoadTexture("red_broken_tile.tx", "red_broken_tile");
			ResourceManager::LoadTexture("brown_tile.tx", "brown_tile");
			ResourceManager::LoadTexture("broken_brown_tile.tx", "brown_broken_tile");
			ResourceManager::LoadTexture("purple_tile.tx", "purple_tile");
			ResourceManager::LoadTexture("purple_broken_tile.tx", "purple_broken_tile");
			ResourceManager::LoadTexture("light_blue_tile.tx", "light_blue_tile");
			ResourceManager::LoadTexture("light_blue_broken_tile.tx", "light_blue_broken_tile");
			ResourceManager::LoadTexture("orange_tile.tx", "orange_tile");
			ResourceManager::LoadTexture("orange_broken_tile.tx", "orange_broken_tile");
			ResourceManager::LoadTexture("ball.tx", "ball");
			ResourceManager::LoadTexture("paddle_00.tx", "paddle_0");
			ResourceManager::LoadTexture("paddle_01.tx", "paddle_1");
			ResourceManager::LoadTexture("paddle_02.tx", "paddle_2");
			ResourceManager::LoadTexture("heart.tx", "heart");
			ResourceManager::LoadTexture("50_extra.tx", "50_extra");
			ResourceManager::LoadTexture("100_extra.tx", "100_extra");
			ResourceManager::LoadTexture("250_extra.tx", "250_extra");
			ResourceManager::LoadTexture("500_extra.tx", "500_extra");
			ResourceManager::LoadTexture("big_paddle.tx", "big_paddle");
			ResourceManager::LoadTexture("small_paddle.tx", "small_paddle");
			ResourceManager::LoadTexture("bigger_paddle_powerup.tx", "bigger_paddle_powerup");
			ResourceManager::LoadTexture("breaks_through_powerup.tx", "breaks_through_powerup");
			ResourceManager::LoadTexture("extra_ball_powerup.tx", "extra_ball_powerup");
			ResourceManager::LoadTexture("fast_powerup.tx", "fast_powerup");
			ResourceManager::LoadTexture("slow_powerup.tx", "slow_powerup");
			ResourceManager::LoadTexture("smaller_paddle_powerup.tx", "smaller_paddle_powerup");
			ResourceManager::LoadTexture("box.tx", "collider");

			//contentLoaded.wait();

		}


		void Init() override
		{
			Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

			graphics.Initialize("Ebony", windowWidth, windowHeight);
			EbonyAudio::AudioManager::Init();
			
			LoadContent();


			graphics.SetMainCamera(camera);
			Ebony::KeyInput::setupKeyInputs(graphics.window);
			Ebony::MouseInput::setupMouseInputs(graphics.window);


			std::vector<int> keys = { GLFW_KEY_E, GLFW_KEY_ESCAPE, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_A, GLFW_KEY_D };
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
			physicsSystem = systems::PhysicsSystem();
			spriteRenderer = systems::SpriteRenderer();




            Shader& s = ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
			ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");



			graphics.InitializeTextDrawing(ResourceManager::GetShader("text"));
			spriteFont.LoadFont("fonts/super-indie-font/SuperIndie.ttf");


            s.use();
            s.setInt("image", 0);
            s.setMat4("projection", graphics.projection);
			clearColor = Colors::CornflowerBlue;
			

			inputSystem = systems::InputSystem();

			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();



			controllerInputComponent->onPressActions.insert({ "quit", [=](entities::EntityPtr) {glfwSetWindowShouldClose(graphics.window.getWindow(), true); } });
			controllerInputComponent->onHeldActions.insert({ "print", [=](entities::EntityPtr) {std::cout << "Circle was called (OnHeld)" << std::endl; } });
			controllerInputComponent->onReleaseActions.insert({ "printRelease", [=](entities::EntityPtr) {std::cout << "Triangle was called" << std::endl; } });
			controllerInputComponent->joystickBindings.insert({ GLFW_GAMEPAD_AXIS_LEFT_X, "paddleMovement" });

			controllerInputComponent->joystickActions.insert({ "paddleMovement", [=](entities::EntityPtr entity, float value) {
				if (abs(value) > 0.10)
				{
					auto rigidBody = entity->getComponent<components::RigidBody>();
					auto transform = entity->getComponent<components::Transform>();
					auto collider = entity->getComponent<components::Collider>();

					if (value < 0 && transform->position.x > 0)
					{
						rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloatMilli() * value, 0.0f });
					}
					else if (value > 0 && transform->position.x < windowWidth)
					{
						rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloatMilli() * value, 0.0f });
					}
					
				}
				
			} });




			//controllerInputComponent->loadControllerBindings("../controllerBindings.json", "../joystickBindings.json");

			//controllerInputComponent->controllerAxes.insert({ GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER, [=](float value) {
			//if (value > -0.5)
			//{
			//	std::cout << "Right Trigger: " << value << std::endl;
			//}
			//} });


			keyboardInputComponent->bindings.insert({ GLFW_KEY_ESCAPE, "quit" });
			keyboardInputComponent->onPressActions.insert({ "quit", [=](entities::EntityPtr entity) {glfwSetWindowShouldClose(graphics.window.getWindow(), true); } });
			//keyboardInputComponent->bindings.insert({ GLFW_KEY_E, "print" });
			keyboardInputComponent->onReleaseActions.insert({ "print", [=](entities::EntityPtr) { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->onHeldActions.insert({ "print", [=](entities::EntityPtr) { std::cout << "E was called" << std::endl; } });

			//keyboardInputComponent->bindings.insert({ GLFW_KEY_LEFT_SHIFT, "toggleRun" });
			//keyboardInputComponent->onReleaseActions.insert({ "toggleRun", [=]() { isRunning = false; } });
			//keyboardInputComponent->onPressActions.insert({ "toggleRun", [=]() { isRunning = true; } });
			keyboardInputComponent->bindings.insert({ GLFW_KEY_A, "paddleLeft" });
			keyboardInputComponent->bindings.insert({ GLFW_KEY_D, "paddleRight" });
			keyboardInputComponent->bindings.insert({ GLFW_KEY_E, "growShrink" });

			keyboardInputComponent->onHeldActions.insert({ "paddleLeft", [=](entities::EntityPtr entity)
			{
				auto rigidBody = entity->getComponent<components::RigidBody>();
				auto transform = entity->getComponent<components::Transform>();
				auto collider = entity->getComponent<components::Collider>();


				if (transform->position.x > 0)
				{
					rigidBody->addScriptedMovement(glm::vec2{ -700.0f * Ebony::Time::GetDeltaTimeFloatMilli(), 0.0f });
				}				
			} });

			keyboardInputComponent->onHeldActions.insert({ "paddleRight", [=](entities::EntityPtr entity)
			{
				auto rigidBody = entity->getComponent<components::RigidBody>();
				auto transform = entity->getComponent<components::Transform>();
				auto collider = entity->getComponent<components::Collider>();
				if (transform->position.x + transform->scale.x < windowWidth)
				{
					rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloatMilli(), 0.0f });
				}
			} 
			});


			keyboardInputComponent->onPressActions.insert({ "growShrink", [=](entities::EntityPtr entity) 
			{
				auto transform = entity->getComponent<components::Transform>();

				if (transform->scale.x == 150.0f)
				{
					transform->scale.x += 50.0f;
				} 
				else
				{
					transform->scale.x = 150.f;
				}

			} });


			//keyboardInputComponent->loadKeyBindings("../keyBindings.json");


			//keyboardInput->addComponent(std::move(controllerInputComponent));
			//keyboardInput->addComponent(std::move(keyboardInputComponent));

			auto textComponent = std::make_unique<components::Text>(fps, Ebony::Colors::Black, Ebony::Colors::White, spriteFont);
			testEntity->addComponent(std::move(textComponent));
			testEntity->addComponent(std::move(std::make_unique<components::Transform>(glm::vec2{ 25.0f, 100.0f }, 0.0f, glm::vec2{ 1.0f, 1.0f })));


			AddEntity(testEntity);

			entities::EntityPtr anotherEntity = std::make_shared<entities::Entity>();
			auto mouseComponent = std::make_unique<components::MouseInput>();

			//mouseComponent->bindings.insert({ GLFW_MOUSE_BUTTON_1, "mousePress" });
			mouseComponent->onPressActions.insert({ "mousePress", [=](entities::EntityPtr) {std::cout << "Button pressed!" << std::endl; }});
			mouseComponent->onReleaseActions.insert({ "mousePress",[=](entities::EntityPtr) {std::cout << "Mouse Button released!" << std::endl; }});
			
			mouseComponent->loadMouseBindings("../mouseBindings.json");
			//mouseComponent->saveMouseBindings("../mouseBindings.json");
			anotherEntity->addComponent(std::move(mouseComponent));
			


			auto sprite = std::make_unique<components::Sprite>(ResourceManager::GetShader("default"), ResourceManager::GetTexture("paddle_0"), Ebony::Colors::White);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(0.0f, 0.0f), glm::vec2(sprite->texture.Width, sprite->texture.Height), true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, 0);
			auto transform = std::make_unique<components::Transform>(glm::vec2(400.0f, 500.0f), 0.0f, glm::vec2(150.0f, 50.0f));
			auto rigidbody = std::make_unique<components::RigidBody>();


			anotherEntity->addComponent(std::move(collider));
			anotherEntity->addComponent(std::move(transform));
			anotherEntity->addComponent(std::move(sprite));
			anotherEntity->addComponent(std::move(keyboardInputComponent));
			anotherEntity->addComponent(std::move(controllerInputComponent));
			anotherEntity->addComponent(std::move(rigidbody));

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

			auto task3 = ThreadPool::instance().createTask(
				taskGraph,
				[this, elapsedTime]()
				{
					physicsSystem.Update(elapsedTime);
				}
			);

			auto task4 = ThreadPool::instance().createTask(
				taskGraph,
				[this, elapsedTime]()
				{
					EbonyAudio::AudioManager::Update();
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


			//physicsSystem.Update(elapsedTime);

			//audioSystem.Update(elapsedTime);

			/*auto previousTime = std::chrono::system_clock::now();
			animationSystem.Update(elapsedTime);
			averageAnimationSystemTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);

			previousTime = std::chrono::system_clock::now();
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
			//graphics.BeginImgui();

			graphics.SetRenderTarget(main, clearColor);
			
			animationRenderer.Update(graphics);
			spriteRenderer.Update(graphics);

			auto previousTime = std::chrono::system_clock::now();
			particleRenderer.Update(graphics);
			averageParticleRenderingTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - previousTime);
			
			fontRenderer.Update(graphics);
			
			graphics.UnbindRenderTarget(clearColor);

			graphics.DrawRenderTarget(ResourceManager::GetShader("screenTexture"), main);


			
			//graphics.DrawWindow("Ebony");
			//graphics.ImguiText("Hello there!");
			//graphics.ImguiText("General Kenobi");

			//ImGui::ArrowButton("Left", ImGuiDir_Left);
			//
			//graphics.CompleteWindow();

			//graphics.EndImgui();
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
				Ebony::Time::SetDeltaTime(elapsedTime);
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
				physicsSystem.AddEntity(entity);
				spriteRenderer.AddEntity(entity);


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
				physicsSystem.RemoveEntity(entityId);
				spriteRenderer.RemoveEntity(entityId);
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
		systems::PhysicsSystem physicsSystem;
		systems::SpriteRenderer spriteRenderer;



		entities::EntityMap allEntities;
		std::vector<entities::EntityPtr> newEntities;
		std::unordered_set<entities::Entity::IdType> removeEntities;
		std::unordered_set<entities::Entity::IdType> updatedEntities;
		std::mutex mutexEntities; // This is necessary so we can add to the newEntities vector even though it is multithreaded

		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		float fpsUpdateDeltaTime = 0.0f;
		int windowWidth = 800;
		int windowHeight = 600;
		std::chrono::microseconds averageParticleRenderingTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageParticleSystemTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageAnimationSystemTime = std::chrono::microseconds::zero();
		std::chrono::microseconds averageUpdateTime = std::chrono::microseconds::zero();

		std::uint64_t totalFrames = 0;
		// SoundDevice, Volume Levels, Sound Loader, Streaming, Source Pools, sounds
		
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