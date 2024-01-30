#include "gameScreen.hpp"
#include "../prefabs/mainMusicPrefab.hpp"

namespace BrickBreaker
{

	void GameScreen::LoadContent()
	{
		Ebony::ResourceManager::LoadSoundEffect("/SoundEffects/wall.wav", "wall",  "", false);
		Ebony::ResourceManager::LoadSoundEffect("click.wav", "click",  "BrickBreaker");
		Ebony::ResourceManager::LoadSoundEffect("ball_bounce.wav", "ball_bounce",  "BrickBreaker");
		Ebony::ResourceManager::LoadSoundEffect("Menu Selection Click.wav", "menu_select", "BrickBreaker");
		Ebony::ResourceManager::LoadSoundEffect("negative.wav", "negative",  "BrickBreaker");
		Ebony::ResourceManager::LoadSoundEffect("positive.wav", "positive",  "BrickBreaker");
		Ebony::ResourceManager::LoadSoundEffect("Powerup.wav", "powerup",  "BrickBreaker");
		Ebony::ResourceManager::LoadSoundEffect("save.wav", "save",  "BrickBreaker");
		Ebony::ResourceManager::LoadSoundEffect("brick_break.wav", "brick_break",  "BrickBreaker");

		Ebony::ResourceManager::LoadMusic("Level1Music.mp3", "base_music", "SpaceGuy");
		Ebony::ResourceManager::LoadMusic("Cyberpunk-Moonlight-Sonata.wav", "cyberpunk_moonlight_sonata",  "BrickBreaker");
		Ebony::ResourceManager::LoadMusic("Cyberpunk-Moonlight-Sonata-editted.wav", "cyberpunk_moonlight_sonata_short", "BrickBreaker");
			
		Ebony::ResourceManager::LoadMusic("song18", "/Music/song18.wav", "", false);
		Ebony::ResourceManager::LoadTexture("Default.tx", "default", "Crypt");
		Ebony::ResourceManager::LoadTexture("BlueBrick.tx", "blue_tile", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("GreenBrick.tx", "green_tile", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("RedBrick.tx", "red_tile", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("PurpleBrick.tx", "purple_tile", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("light_blue_broken_tile.tx", "light_blue_broken_tile", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("Ball.tx", "ball", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("Paddle.tx", "paddle_0", "BrickBreaker");
		//Ebony::ResourceManager::LoadTexture("big_paddle.tx", "big_paddle", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("WidePowerup.tx", "bigger_paddle_powerup", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("box.tx", "collider", "BrickBreaker");
		Ebony::ResourceManager::LoadTexture("empty.tx", "empty", "BrickBreaker");
		Ebony::ResourceManager::LoadFont("super-indie-font/SuperIndie.ttf", "default", "Crypt");


		//Ebony::ResourceManager::LoadTexture("Button_Unpressed.tx", "button_unpressed");
		//Ebony::ResourceManager::LoadTexture("Button_Pressed.tx", "button_pressed");
		//Ebony::ResourceManager::LoadTexture("Button_Unpressed.tx", "button_unpressed");
		//Ebony::ResourceManager::LoadTexture("Button_Hovered.tx", "button_hovered");
		//Ebony::ResourceManager::LoadTexture("Logo_BrickBreaker.tx", "logo_brickbreaker");
	}

	


	void GameScreen::Init(int windowWidth, int windowHeight)
	{
		camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));
		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
		mainRenderTarget = Ebony::RenderTarget2D::Create(Ebony::Graphics2d::screenWidth, Ebony::Graphics2d::screenHeight, GL_LINEAR, GL_NEAREST);

		clearColor = Ebony::Colors::CornflowerBlue;

		Ebony::Graphics2d::SetMainCamera(camera);
	}

	void GameScreen::Start()
	{
		auto testParticles = std::make_shared<entities::Entity>();
		auto keyboardInput = std::make_shared<entities::Entity>();
		auto animationsTest = std::make_shared<entities::Entity>();

		particleSystem = systems::ParticleSystem();
		particleRenderer = systems::ParticleRenderer();
		animationRenderer = systems::AnimationRenderer();
		animationSystem = systems::Animation2d();
		fontRenderer = systems::FontRenderer();
		audioSystem = systems::AudioSystem();
		physicsSystem = systems::PhysicsSystem();
		spriteRenderer = systems::SpriteRenderer();
		ballSystem = systems::BallSystem();
		inputSystem = systems::InputSystem();
		lifePointSystem = systems::LifePointSystem();
		cppScriptingSystem = systems::CppScriptingSystem();
		destructionSystem = systems::DestructionSystem();
		timingSystem = systems::TimingSystem();

		std::shared_ptr<Shader> s = Ebony::ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
		Ebony::ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");
		//spriteFont = std::make_shared<Ebony::SpriteFont>();


		Ebony::Graphics2d::InitializeTextDrawing(Ebony::ResourceManager::GetShader("text"));
		//spriteFont->LoadFont("../Graphics/fonts/super-indie-font/SuperIndie.ttf");


		s->use();
		s->setInt("image", 0);
		s->setMat4("projection", Ebony::Graphics2d::projection);

		std::unique_ptr<components::ControllerInput> gameplayControllerInputComponent = std::make_unique<components::ControllerInput>(0);
		std::unique_ptr<components::KeyboardInput> gameplayKeyboardInputComponent = std::make_unique<components::KeyboardInput>();

		entities::EntityPtr gameplayEntity = std::make_shared<entities::Entity>();
		// ================================================================================================================================================================
		// TODO:
		// ================================================================================================================================================================
		/*gameplayControllerInputComponent->onPressActions.insert({ "quit", [=](entities::EntityPtr) {
			glfwSetWindowShouldClose(Ebony::Graphics2d::window.getWindow(), true);
			}
		});*/

		gameplayControllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_START, "quit" });
		// ================================================================================================================================================================
		// TODO:
		// ================================================================================================================================================================
		/*gameplayKeyboardInputComponent->onPressActions.insert({ "quit", [=](entities::EntityPtr entity)
			{
				glfwSetWindowShouldClose(Ebony::Graphics2d::window.getWindow(), true);
			}
		});*/

		gameplayKeyboardInputComponent->bindings.insert({ SDLK_ESCAPE, "quit" });

		gameplayEntity->addComponent(std::move(gameplayControllerInputComponent));
		gameplayEntity->addComponent(std::move(gameplayKeyboardInputComponent));


		//gameplayEntity->addComponent(std::make_unique<components::Music>(Ebony::IndividualMusicTrack(Ebony::ResourceManager::GetMusic("cyberpunk_moonlight_sonata_short"), 127)));


		AddEntity(gameplayEntity);

		entities::EntityPtr paddleEntity = BrickBreaker::Paddle::Create(glm::vec2(20.0f, 300.0f), windowWidth);
		AddEntity(paddleEntity);

		entities::EntityPtr ballEntity = BrickBreaker::Ball::Create(glm::vec2(20.0f, 280.0f), paddleEntity, true);
		AddEntity(ballEntity);

		for (std::uint8_t i = 0; i < 27; i++)
		{
			entities::EntityPtr brickEntity = BrickBreaker::Brick::Create(30.0f + 16.0f * i, 23.0f, "blue_tile", 4, 40);
			AddEntity(brickEntity);
		}

		for (std::uint8_t i = 0; i < 27; i++)
		{
			entities::EntityPtr brickEntity = BrickBreaker::Brick::Create(30.0f + 16.0f * i, 31.0f, "green_tile", 3, 30);
			AddEntity(brickEntity);
		}

		for (std::uint8_t i = 0; i < 27; i++)
		{
			entities::EntityPtr brickEntity = BrickBreaker::Brick::Create(30.0f + 16.0f * i, 39.0f, "red_tile", 2, 20);
			AddEntity(brickEntity);
		}

		for (std::uint8_t i = 0; i < 27; i++)
		{
			entities::EntityPtr brickEntity = BrickBreaker::Brick::Create(30.0f + 16.0f * i, 47.0f, "purple_tile", 1, 10);
			AddEntity(brickEntity);
		}

		for (std::uint8_t i = 0; i < 27; i++)
		{
			entities::EntityPtr brickEntity = BrickBreaker::Brick::Create(30.0f + 16.0f * i, 55.0f, "purple_tile", 1, 10);
			AddEntity(brickEntity);
		}

		entities::EntityPtr scoreDisplay = BrickBreaker::ScorePrefab::Create(glm::vec2(windowWidth - 150.0f, 25.0f), Ebony::ResourceManager::GetFont("default"));

		AddEntity(scoreDisplay);


		//keyboardInputComponent->loadKeyBindings("../keyBindings.json");

		//keyboardInput->getComponent<components::KeyboardInput>()->saveKeyBindings("../keyBindings.json");
		//keyboardInput->getComponent<components::ControllerInput>()->saveControllerBindings("../controllerBindings.json", "../joystickBindings.json");

		AddEntity(BrickBreaker::MainMusicPrefab::Create("cyberpunk_moonlight_sonata_short", 127));


		entities::EntityPtr rightWall = BrickBreaker::Wall::Create(static_cast<float>(windowWidth) - 5.0f, 0.0f, 10.0f, static_cast<float>(windowHeight), BrickBreaker::CollisionLayers::WALL);
		entities::EntityPtr leftWall = BrickBreaker::Wall::Create(0.0f, 0.0f, 10.0f, static_cast<float>(windowHeight), BrickBreaker::CollisionLayers::WALL);
		entities::EntityPtr topWall = BrickBreaker::Wall::Create(0.0f, 0.0f, static_cast<float>(windowWidth), 10.0f, BrickBreaker::CollisionLayers::WALL);
		entities::EntityPtr bottomWall = BrickBreaker::Wall::Create(0.0f, static_cast<float>(windowHeight) - 10.0f, static_cast<float>(windowWidth), 40.0f, BrickBreaker::CollisionLayers::BOTTOM_WALL);
		
		AddEntity(rightWall);
		AddEntity(leftWall);
		AddEntity(topWall);


		AddEntity(bottomWall);

		AddNewEntities();
	}
		
	void GameScreen::AddNewEntities()
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
			ballSystem.AddEntity(entity);
			lifePointSystem.AddEntity(entity);
			cppScriptingSystem.AddEntity(entity);
			timingSystem.AddEntity(entity);
			destructionSystem.AddEntity(entity);

			allEntities[entity->getId()] = entity;
		}

		newEntities.clear();
	}
		
	void GameScreen::RemoveOldEntities()
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
			ballSystem.RemoveEntity(entityId);
			lifePointSystem.RemoveEntity(entityId);
			cppScriptingSystem.RemoveEntity(entityId);
			timingSystem.RemoveEntity(entityId);
			destructionSystem.RemoveEntity(entityId);

		}

		removeEntities.clear();
	}
		
	std::uint64_t GameScreen::Update(std::chrono::microseconds elapsedTime)
	{
		auto firstTime = std::chrono::system_clock::now();
		

		std::latch graphDone{ 1 };

		auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
			[&graphDone]()
			{
				graphDone.count_down();
			});

		auto animationUpdate = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				animationSystem.Update(elapsedTime);
			}
		);

		auto particleUpdate = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				particleSystem.Update(elapsedTime);
			}
		);


		auto audioUpdate = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				audioSystem.Update(elapsedTime);
			}
		);

		auto ballUpdate = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				ballSystem.Update(elapsedTime);
			}
		);


		auto lifeUpdate = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				lifePointSystem.Update(elapsedTime);
			}
		);

		auto scriptingUpdate = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				cppScriptingSystem.Update(elapsedTime);
			}
		);

		auto timerTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				timingSystem.Update(elapsedTime);
			}
		);

		auto destructionTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				destructionSystem.Update(elapsedTime);
			}
		);


		// Declare predecessors here
		//taskGraph->declarePredecessor(task6->getId(), task4->getId());
		//taskGraph->declarePredecessor(task7->getId(), task4->getId());
		//taskGraph->declarePredecessor(task5->getId(), task4->getId());
		//taskGraph->declarePredecessor(task3->getId(), task4->getId());
		//taskGraph->declarePredecessor(task2->getId(), task4->getId());
		//taskGraph->declarePredecessor(task1->getId(), task4->getId());



		Ebony::ThreadPool::instance().submitTaskGraph(taskGraph);
		graphDone.wait();

		physicsSystem.Update(elapsedTime);

		averageUpdateTime += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - firstTime);

		return nextScreen;

	}
		
	void GameScreen::Draw(std::chrono::microseconds elapsedTime)
	{
			
		//Ebony::Graphics2d::BeginImgui();

		Ebony::Graphics2d::SetRenderTarget(mainRenderTarget, clearColor);

		animationRenderer.Update();
		spriteRenderer.Update();

		auto previousTime = std::chrono::system_clock::now();
		particleRenderer.Update();
		fontRenderer.Update();


		Ebony::Graphics2d::UnbindRenderTarget(clearColor);

		Ebony::Graphics2d::DrawRenderTarget(Ebony::ResourceManager::GetShader("screenTexture"), mainRenderTarget);
	}
	void GameScreen::ProcessInput(std::chrono::microseconds elapsedTime)
	{
		inputSystem.Update();
	}

	void GameScreen::AddEntity(entities::EntityPtr entity)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		newEntities.push_back(entity);
	}

	void GameScreen::RemoveEntity(entities::Entity::IdType id)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		removeEntities.insert(id);
	}

	void GameScreen::Reset()
	{
		// Delete all entities in play
		RemoveAllEntities();
		Start();
	}

	void GameScreen::RemoveAllEntities()
	{
		for (auto&& [entityId, entity] : allEntities)
		{
			particleSystem.RemoveEntity(entityId);
			particleRenderer.RemoveEntity(entityId);
			animationRenderer.RemoveEntity(entityId);
			animationSystem.RemoveEntity(entityId);
			inputSystem.RemoveEntity(entityId);
			audioSystem.RemoveEntity(entityId);
			fontRenderer.RemoveEntity(entityId);
			physicsSystem.RemoveEntity(entityId);
			spriteRenderer.RemoveEntity(entityId);
			ballSystem.RemoveEntity(entityId);
			lifePointSystem.RemoveEntity(entityId);
			cppScriptingSystem.RemoveEntity(entityId);
			destructionSystem.RemoveEntity(entityId);
			timingSystem.RemoveEntity(entityId);
		}

		allEntities.clear();
	}

	void GameScreen::OnScreenDefocus(std::uint64_t nextScreenEnum)
	{
		nextScreen = screen;

		std::cout << "Defocusing Game Screen" << std::endl;
		Ebony::AudioManager::StopAll();
	}

	void GameScreen::OnScreenFocus(std::uint64_t lastScreenEnum)
	{
		std::cout << "Focusing Game Screen" << std::endl;
		Reset();
	}

}