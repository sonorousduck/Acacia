#include "mainGameScreen.hpp"


#include "../prefabs/mainMusicPrefab.hpp"
#include "../prefabs/player.hpp"
#include "../prefabs/smallEnemyPrefab.hpp"
#include "../prefabs/largerEnemyPrefab.hpp"


namespace SpaceGuy
{

	void MainGameScreen::LoadContent()
	{
		



		Ebony::ResourceManager::LoadFont("super-indie-font/SuperIndie.ttf", "default", "SpaceGuy");
		Ebony::ResourceManager::LoadFont("evil-empire-font/EvilEmpire-4BBVK.ttf", "evil_empire_36", "SpaceGuy", 36, 36);
		Ebony::ResourceManager::LoadFont("evil-empire-font/EvilEmpire-4BBVK.ttf", "evil_empire_24", "SpaceGuy", 24, 24);
		Ebony::ResourceManager::LoadFont("evil-empire-font/EvilEmpire-4BBVK.ttf", "evil_empire_12", "SpaceGuy", 12, 12);
		Ebony::ResourceManager::LoadTexture("Default.tx", "default", "Crypt");

		// Tiles
		Ebony::ResourceManager::LoadTexture("PlayerSpaceship.tx", "player", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("Enemy1.tx", "basic_enemy", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("Enemy2.tx", "advanced_enemy", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("EnemyBullet.tx", "enemy_bullet", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("EnemyRadar.tx", "enemy_radar", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("ExplosiveBarrel.tx", "explosive_barrel", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("FastShooting.tx", "fast_shooting_powerup", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("Key.tx", "key", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("Missile.tx", "missile", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("PlayerBullet.tx", "player_bullet", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("PlayerLife.tx", "player_life", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("PlayerRadar.tx", "player_radar", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("Potion.tx", "potion", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("RadarBackground.tx", "radar_background", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("ShotSpread.tx", "shot_spread_powerup", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("Spawner.tx", "spawner", "SpaceGuy");
		Ebony::ResourceManager::LoadTexture("SpeedBoost.tx", "speed_boost", "SpaceGuy");


		Ebony::ResourceManager::LoadAtlas("Exploding Red Oil Barrel.tx", "explosion", "SpaceGuy", 12, 1);


		Ebony::ResourceManager::LoadSoundEffect("EnemyShoot.wav", "enemy_shoot", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("explosion.wav", "explosion", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("hurtSoundVariation.wav", "hurt_sound_variation", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("hurtSound.wav", "hurt_sound", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("laserShoot.wav", "laser_shoot", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("powerUp.wav", "powerup", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("RegEnemyDeath.wav", "enemy_death", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("SpawnerDeath.wav", "spawner_death", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("missileShoot.wav", "missile_shoot", "SpaceGuy");
		Ebony::ResourceManager::LoadSoundEffect("laserShootVariation.wav", "laser_shoot_variation", "SpaceGuy");


		Ebony::ResourceManager::LoadMusic("Level1Music.mp3", "base_music", "SpaceGuy");

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

		physicsSystem = systems::PhysicsSystem();
		spriteRenderer = systems::SpriteRenderer();
		animationSystem = systems::Animation2d();
		fontRenderer = systems::FontRenderer();
		animationRenderer = systems::AnimationRenderer();
		inputSystem = systems::InputSystem();
		audioSystem = systems::AudioSystem();
		cppScriptingSystem = systems::CppScriptingSystem();
		timingSystem = systems::TimingSystem();
		destructionSystem = systems::DestructionSystem();
		pythonScriptingSystem = systems::PythonScriptingSystem();
		aiInputSystem = systems::AIInputSystem();
		aiSystem = systems::AISystem();
		enemyDetectionSystem = systems::EnemyDetectionSystem();

		spriteRenderer.debug = false;





		// Load Tiled map
		//SpaceGuyTiledProcessor tiledProcessor = SpaceGuyTiledProcessor(player);
		//tiledProcessor.CreateTranslationFunction();
		//tiledProcessor.ParseMap("../SpaceGuy/maps/SpaceGuy/SpaceGuy.json");


		// Create prefabs

		AddEntity(SpaceGuy::MainMusicPrefab::Create("base_music", 5));
		

		auto player = SpaceGuy::Player::Create();

		AddEntity(player);
		AddEntity(SpaceGuy::SmallEnemy::Create(glm::vec2(100.0f, 50.0f), glm::vec2(1.0f, 1.0f), player));
		// For now, pretend we clicked on the mountains store tile (until I find a good way to render conditionally) (Maybe just outright removing and replacing, but that might be a lot of unnecessary work)


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
		pythonScriptingSystem.Update(elapsedTime);

		auto firstTime = std::chrono::system_clock::now();
		//Ebony::Graphics2d::mainCamera->Position = glm::vec3(Ebony::Graphics2d::mainCamera->Position.x + 0.01f, Ebony::Graphics2d::mainCamera->Position.y, Ebony::Graphics2d::mainCamera->Position.z);


		std::latch graphDone{ 1 };

		auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
			[&graphDone]()
			{
				graphDone.count_down();
			});

		auto audioTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				audioSystem.Update(elapsedTime);
			}
		);

		auto inputTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				inputSystem.Update();
			}
		);

		auto animationTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				animationSystem.Update(elapsedTime);
			}
		);

		auto scriptingTask = Ebony::ThreadPool::instance().createTask(
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


		/*if (Ebony::SystemManager::shouldResetForAi)
		{
			SpaceGuy::EcologyPythonManager::Reset();
			RemoveAllEntities();
			RemoveOldEntities();
			Start();
			Ebony::SystemManager::shouldResetForAi = false;
			return nextScreen;
		}*/


		// Put this outside the update loop so in the future, I can use all the threads to then do multi-threaded physics updates
		physicsSystem.Update(elapsedTime);
		//SpaceGuy::SpaceGuyPythonManager::Update(elapsedTime);

		


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
		aiInputSystem.Update(elapsedTime);
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
		//SDL_ShowCursor(SDL_ENABLE);
		nextScreen = screen;
		
		if (Ebony::SystemManager::nextScreenEnum == SpaceGuy::ScreenEnum::PAUSE)
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
		//SDL_ShowCursor(SDL_DISABLE);


		if (Ebony::SystemManager::lastScreenEnum == SpaceGuy::ScreenEnum::PAUSE)
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
			cppScriptingSystem.AddEntity(entity);
			destructionSystem.AddEntity(entity);
			timingSystem.AddEntity(entity);
			fontRenderer.AddEntity(entity);
			pythonScriptingSystem.AddEntity(entity);
			aiInputSystem.AddEntity(entity);
			aiSystem.AddEntity(entity);
			enemyDetectionSystem.AddEntity(entity);

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
			cppScriptingSystem.RemoveEntity(entityId);
			destructionSystem.RemoveEntity(entityId);
			timingSystem.RemoveEntity(entityId);
			fontRenderer.RemoveEntity(entityId);
			pythonScriptingSystem.RemoveEntity(entityId);
			aiInputSystem.RemoveEntity(entityId);
			aiSystem.RemoveEntity(entityId);
			enemyDetectionSystem.RemoveEntity(entityId);
		}

		removeEntities.clear();
	}

	void MainGameScreen::RemoveAllEntities()
	{
		for (auto&& [entity, entityId] : allEntities)
		{
			RemoveEntity(entity);
		}


	}

}