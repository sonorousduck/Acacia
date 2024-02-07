#include "mainGameScreen.hpp"

#include "../prefabs/playerPrefab.hpp"
#include "../prefabs/groundPrefab.hpp"
#include "../prefabs/crosshairPrefab.hpp"
#include "../prefabs/batPrefab.hpp"
#include "../prefabs/suicideBirdPrefab.hpp"

#include "../prefabs/UI/playerHealthPrefab.hpp"
#include "../prefabs/UI/playerSpellSelection.hpp"
#include "../prefabs/mainMusicPrefab.hpp"
#include "../prefabs/gameCompletePrefab.hpp"

namespace Crypt
{

	void MainGameScreen::LoadContent()
	{
		Ebony::ResourceManager::LoadAtlas("character_run.tx", "character_run", "Crypt", 4, 1);
		Ebony::ResourceManager::LoadTexture("Default.tx", "default", "Crypt");
		Ebony::ResourceManager::LoadTexture("particle.tx", "collider", "Crypt");
		Ebony::ResourceManager::LoadTexture("empty.tx", "empty", "Crypt");
		Ebony::ResourceManager::LoadTexture("new_aim.tx", "aim", "Crypt");
		Ebony::ResourceManager::LoadTexture("FireBall.tx", "fire_bullet", "Crypt");
		Ebony::ResourceManager::LoadTexture("FireBallPanel.tx", "fire_bullet_panel", "Crypt");

		Ebony::ResourceManager::LoadTexture("IceBolt.tx", "ice_bullet", "Crypt");
		Ebony::ResourceManager::LoadTexture("IceBoltPanel.tx", "ice_bullet_panel", "Crypt");

		Ebony::ResourceManager::LoadTexture("Bat1.tx", "bat", "Crypt");
		Ebony::ResourceManager::LoadTexture("BatAttack.tx", "bat_attack", "Crypt");
		Ebony::ResourceManager::LoadTexture("Icon.tx", "icon", "Crypt");
		Ebony::ResourceManager::LoadTexture("wood.tx", "wood", "Crypt");

		Ebony::ResourceManager::LoadAtlas("Exploding Red Oil Barrel.tx", "explosion", "Crypt", 12, 1);
		Ebony::ResourceManager::LoadAtlas("Test_SpriteSheet.tx", "test_spritesheet", "Crypt", 4, 4);
		Ebony::ResourceManager::LoadAtlas("CryptTileset.tx", "crypt_spritesheet", "Crypt", 24, 24);
		Ebony::ResourceManager::LoadAtlas("BomberBird.tx", "bomber_bird", "Crypt", 6, 1);

		Ebony::ResourceManager::LoadTexture("Panel.tx", "panel", "Crypt");
		Ebony::ResourceManager::LoadTexture("Heart.tx", "full_heart", "Crypt");
		Ebony::ResourceManager::LoadTexture("DamagedHeart.tx", "damaged_heart", "Crypt");
		Ebony::ResourceManager::LoadTexture("BrokenHeart.tx", "broken_heart", "Crypt");
		Ebony::ResourceManager::LoadTexture("HeartUIBackground.tx", "heart_ui_background", "Crypt");



		Ebony::ResourceManager::LoadFont("super-indie-font/SuperIndie.ttf", "default", "Crypt");
		Ebony::ResourceManager::LoadFont("evil-empire-font/EvilEmpire-4BBVK.ttf", "evil_empire_36", "Crypt", 36, 36);
		Ebony::ResourceManager::LoadFont("evil-empire-font/EvilEmpire-4BBVK.ttf", "evil_empire_24", "Crypt", 24, 24);
		Ebony::ResourceManager::LoadFont("evil-empire-font/EvilEmpire-4BBVK.ttf", "evil_empire_12", "Crypt", 12, 12);


		Ebony::ResourceManager::LoadSoundEffect("explosion.wav", "spell", "Crypt");

		Ebony::ResourceManager::LoadMusic("commonFight.ogg", "base_music", "Crypt");

	}

	void MainGameScreen::Start()
	{
		std::shared_ptr<Shader> s = Ebony::ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
		Ebony::ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");

		Ebony::Graphics2d::InitializeTextDrawing(Ebony::ResourceManager::GetShader("text"));
		this->clearColor = Ebony::Colors::CornflowerBlue;


		s->use();
		s->setInt("image", 0);
		s->setMat4("projection", Ebony::Graphics2d::projection);

		physicsSystem = systems::PhysicsSystem(glm::vec2{ 10000, 1000 }, glm::vec2{ 50, 50 }, glm::vec2(-200, -100));
		spriteRenderer = systems::SpriteRenderer();
		animationSystem = systems::Animation2d();
		fontRenderer = systems::FontRenderer();
		animationRenderer = systems::AnimationRenderer();
		playerSystem = systems::PlayerSystem();
		inputSystem = systems::InputSystem();
		audioSystem = systems::AudioSystem();
		cppScriptingSystem = systems::CppScriptingSystem();
		timingSystem = systems::TimingSystem();
		destructionSystem = systems::DestructionSystem();
		enemyDetectionSystem = systems::EnemyDetectionSystem();
		pythonScriptingSystem = systems::PythonScriptingSystem();
		aiInputSystem = systems::AIInputSystem();
		aiSystem = systems::AISystem();

		spriteRenderer.debug = false;




		auto player = Crypt::Player::Create(glm::vec2(20.0f, 80.0f), [=](std::uint64_t nextScreen) { SetNextScreen(nextScreen); });

		// Load Tiled map
		CryptTiledProcessor tiledProcessor = CryptTiledProcessor(player);
		tiledProcessor.CreateTranslationFunction();
		tiledProcessor.ParseMap("../Crypt/maps/crypt/Crypt.json");


		auto crosshair = Crypt::Crosshair::Create(glm::vec2(25.0f, 0.0f), player);
		// Create prefabs
		AddEntity(player);
		AddEntity(Crypt::Ground::Create(glm::vec2(0.0f, static_cast<float>(windowHeight) - 64.0f), static_cast<float>(windowWidth) * 30.0f));
		AddEntity(Crypt::Ground::Create(glm::vec2(0.0f, 48.0f), static_cast<float>(windowWidth) * 30.0f));
		AddEntity(crosshair);


		AddEntity(Crypt::GameComplete::Create(glm::vec2(3550.0f, 0.0f), static_cast<float>(windowHeight)));

		//AddEntity(Crypt::Bat::Create(glm::vec2(200.0f, 80.0f), glm::vec2(1.0f, 1.0f), player));

		AddEntity(Crypt::MainMusicPrefab::Create("base_music", 5));

		AddEntity(Crypt::PlayerHealth::Create(player));
		AddEntity(Crypt::PlayerSpellSelection::Create(windowHeight, crosshair));
		
		AddNewEntities();
	}

	void MainGameScreen::Init(int windowWidth, int windowHeight)
	{
		camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));

		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
		mainRenderTarget = Ebony::RenderTarget2D::Create(windowWidth, windowHeight, GL_LINEAR, GL_NEAREST);
		clearColor = Ebony::Colors::Black;

		Ebony::Graphics2d::SetMainCamera(camera);
	}

	

	std::uint64_t MainGameScreen::Update(std::chrono::microseconds elapsedTime)
	{
		//pythonScriptingSystem.Update(elapsedTime);

		auto firstTime = std::chrono::system_clock::now();
		//Ebony::Graphics2d::mainCamera->Position = glm::vec3(Ebony::Graphics2d::mainCamera->Position.x + 0.01f, Ebony::Graphics2d::mainCamera->Position.y, Ebony::Graphics2d::mainCamera->Position.z);


		std::latch graphDone{ 1 };

		auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
			[&graphDone]()
			{
				graphDone.count_down();
			});

		if (!Ebony::SystemManager::aiEnabled)
		{
			auto audioTask = Ebony::ThreadPool::instance().createTask(
				taskGraph,
				[this, elapsedTime]()
				{
					audioSystem.Update(elapsedTime);
				}
			);
		}

		auto animationTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				animationSystem.Update(elapsedTime);
			}
		);

		auto playerTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				playerSystem.Update(elapsedTime);
			}
		);

		auto scriptingTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				cppScriptingSystem.Update(elapsedTime);
			}
		);

		auto shootingTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				shootingSystem.Update(elapsedTime);
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

		auto enemyDetectionTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				enemyDetectionSystem.Update(elapsedTime);
			}
		);

		if (Ebony::SystemManager::aiEnabled)
		{
			auto aiSystemTask = Ebony::ThreadPool::instance().createTask(
				taskGraph,
				[this, elapsedTime]()
				{
					aiSystem.Update(elapsedTime);
				}
			);
		}
		

		Ebony::ThreadPool::instance().submitTaskGraph(taskGraph);
		graphDone.wait();


		if (Ebony::SystemManager::shouldResetForAi)
		{
			//Crypt::CryptPythonManager::Reset();
			RemoveAllEntities();
			RemoveOldEntities();
			//Start();
			//Ebony::SystemManager::shouldResetForAi = false;
			return nextScreen;
		}


		// Put this outside the update loop so in the future, I can use all the threads to then do multi-threaded physics updates
		physicsSystem.Update(elapsedTime);


		return nextScreen;
	}

	void MainGameScreen::Draw(std::chrono::microseconds elapsedTime)
	{
		Ebony::Graphics2d::SetRenderTarget(mainRenderTarget, clearColor);

		// Draw things!
		spriteRenderer.Update();
		animationRenderer.Update();
		fontRenderer.Update();

		Ebony::Graphics2d::UnbindRenderTarget(clearColor);

		Ebony::Graphics2d::DrawRenderTarget(Ebony::ResourceManager::GetShader("screenTexture"), mainRenderTarget);

	}

	void MainGameScreen::ProcessInput(std::chrono::microseconds elapsedTime)
	{
		inputSystem.Update();

		if (Ebony::SystemManager::aiEnabled)
		{
			aiInputSystem.Update(elapsedTime);
		}
	}

	void MainGameScreen::AddEntity(entities::EntityPtr entity)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		newEntities.push_back(entity);
	}

	void MainGameScreen::RemoveEntity(entities::Entity::IdType id)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		removeEntities.insert(id);
	}

	void MainGameScreen::OnScreenDefocus(std::uint64_t nextScreenEnum)
	{
		nextScreen = screen;
		
		if (Ebony::SystemManager::nextScreenEnum == Crypt::ScreenEnum::PAUSE)
		{
			Ebony::AudioManager::PauseAll();
		}
		else
		{
			Ebony::AudioManager::StopAll();
		}

		
	}

	void MainGameScreen::OnScreenFocus(std::uint64_t lastScreenEnum)
	{
		if (Ebony::SystemManager::lastScreenEnum == Crypt::ScreenEnum::PAUSE)
		{
			Ebony::AudioManager::UnpauseAll();
		}
		else
		{
			Start();
		}


		Ebony::Graphics2d::SetMainCamera(camera);
	}

    void MainGameScreen::SetNextScreen(std::uint64_t nextScreenIncoming)
	{
		nextScreen = nextScreenIncoming;
	}


	void MainGameScreen::AddNewEntities()
	{
		for (auto&& entity : newEntities)
		{
			spriteRenderer.AddEntity(entity);
			physicsSystem.AddEntity(entity);
			audioSystem.AddEntity(entity);
			inputSystem.AddEntity(entity);
			animationSystem.AddEntity(entity);
			animationRenderer.AddEntity(entity);
			playerSystem.AddEntity(entity);
			cppScriptingSystem.AddEntity(entity);
			shootingSystem.AddEntity(entity);
			destructionSystem.AddEntity(entity);
			timingSystem.AddEntity(entity);
			enemyDetectionSystem.AddEntity(entity);
			fontRenderer.AddEntity(entity);
			pythonScriptingSystem.AddEntity(entity);
			aiInputSystem.AddEntity(entity);
			aiSystem.AddEntity(entity);

			allEntities[entity->getId()] = entity;
		}
		newEntities.clear();
	}

	void MainGameScreen::RemoveOldEntities()
	{
		for (auto&& entityId : removeEntities)
		{
			allEntities.erase(entityId);
			spriteRenderer.RemoveEntity(entityId);
			physicsSystem.RemoveEntity(entityId);
			audioSystem.RemoveEntity(entityId);
			inputSystem.RemoveEntity(entityId);
			animationSystem.RemoveEntity(entityId);
			animationRenderer.RemoveEntity(entityId);
			playerSystem.RemoveEntity(entityId);
			cppScriptingSystem.RemoveEntity(entityId);
			shootingSystem.RemoveEntity(entityId);
			destructionSystem.RemoveEntity(entityId);
			timingSystem.RemoveEntity(entityId);
			enemyDetectionSystem.RemoveEntity(entityId);
			fontRenderer.RemoveEntity(entityId);
			pythonScriptingSystem.RemoveEntity(entityId);
			aiInputSystem.RemoveEntity(entityId);
			aiSystem.RemoveEntity(entityId);
		}

		removeEntities.clear();
	}

	void MainGameScreen::RemoveAllEntities()
	{
		for (auto&& [entityId, entity] : allEntities)
		{
			spriteRenderer.RemoveEntity(entityId);
			physicsSystem.RemoveEntity(entityId);
			audioSystem.RemoveEntity(entityId);
			inputSystem.RemoveEntity(entityId);
			animationSystem.RemoveEntity(entityId);
			animationRenderer.RemoveEntity(entityId);
			playerSystem.RemoveEntity(entityId);
			cppScriptingSystem.RemoveEntity(entityId);
			shootingSystem.RemoveEntity(entityId);
			destructionSystem.RemoveEntity(entityId);
			timingSystem.RemoveEntity(entityId);
			enemyDetectionSystem.RemoveEntity(entityId);
			fontRenderer.RemoveEntity(entityId);
			pythonScriptingSystem.RemoveEntity(entityId);
			aiInputSystem.RemoveEntity(entityId);
			aiSystem.RemoveEntity(entityId);
		}

		allEntities.clear();
	}

	void MainGameScreen::Reset()
	{
		RemoveAllEntities();
		Start();
	}

}