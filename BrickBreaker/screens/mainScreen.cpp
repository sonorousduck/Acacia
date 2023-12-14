#include "mainScreen.hpp"

#include "../prefabs/startButtonPrefab.hpp"


namespace BrickBreaker
{

	void MainScreen::LoadContent()
	{
		Ebony::ResourceManager::LoadTexture("Button_Unpressed.tx", "button_unpressed");
		Ebony::ResourceManager::LoadTexture("Button_Pressed.tx", "button_pressed");
		Ebony::ResourceManager::LoadTexture("Button_Hovered.tx", "button_hovered");
		Ebony::ResourceManager::LoadTexture("Logo_BrickBreaker.tx", "logo_brickbreaker");
		Ebony::ResourceManager::LoadTexture("Start_Text.tx", "start_text");
		Ebony::ResourceManager::LoadTexture("Options_Text.tx", "options_text");
		Ebony::ResourceManager::LoadTexture("Quit_Text.tx", "quit_text");
	}

	void MainScreen::Init(int windowWidth, int windowHeight)
	{
		Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));

		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
		mainRenderTarget = Ebony::RenderTarget2D::Create(windowWidth, windowHeight, GL_LINEAR, GL_NEAREST);
		clearColor = Ebony::Colors::CornflowerBlue;

		LoadContent();

		Ebony::Graphics2d::SetMainCamera(camera);

		Shader& s = Ebony::ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
		Ebony::ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");

		Ebony::Graphics2d::InitializeTextDrawing(Ebony::ResourceManager::GetShader("text"));


		s.use();
		s.setInt("image", 0);
		s.setMat4("projection", Ebony::Graphics2d::projection);

		physicsSystem = systems::PhysicsSystem();
		spriteRenderer = systems::SpriteRenderer();
		inputSystem = systems::InputSystem();
		audioSystem = systems::AudioSystem();

		// Create prefabs

		AddEntity(BrickBreaker::StartButton::Create(0.0f, 50.0f, 1.0f, 1.0f, "logo_brickbreaker", "button_hovered", "button_pressed"));


		AddNewEntities();

	}

	void MainScreen::AddNewEntities()
	{
		for (auto&& entity : newEntities)
		{
			spriteRenderer.AddEntity(entity);
			physicsSystem.AddEntity(entity);
			audioSystem.AddEntity(entity);
			inputSystem.AddEntity(entity);

			allEntities[entity->getId()] = entity;
		}
		newEntities.clear();
	}

	void MainScreen::RemoveOldEntities()
	{
		for (auto&& entityId : removeEntities)
		{
			allEntities.erase(entityId);
			spriteRenderer.RemoveEntity(entityId);
			physicsSystem.RemoveEntity(entityId);
			audioSystem.RemoveEntity(entityId);
			inputSystem.RemoveEntity(entityId);

		}

		removeEntities.clear();
	}

	std::uint16_t MainScreen::Update(std::chrono::microseconds elapsedTime)
	{
		auto firstTime = std::chrono::system_clock::now();

		std::latch graphDone{ 1 };

		auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
			[&graphDone]()
			{
				graphDone.count_down();
			});

		// UI will need physics layer, input system, music, sprite

		auto physicsTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				physicsSystem.Update(elapsedTime);
			}
		);

		auto audioTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				audioSystem.Update(elapsedTime);
			}
		);


		Ebony::ThreadPool::instance().submitTaskGraph(taskGraph);
		graphDone.wait();


		return nextScreen;

	}

	void MainScreen::Draw(std::chrono::microseconds elapsedTime)
	{
		Ebony::Graphics2d::SetRenderTarget(mainRenderTarget, clearColor);

		// Draw things!
		spriteRenderer.Update();

		Ebony::Graphics2d::UnbindRenderTarget(clearColor);

		Ebony::Graphics2d::DrawRenderTarget(Ebony::ResourceManager::GetShader("screenTexture"), mainRenderTarget);
	}

	void MainScreen::ProcessInput(std::chrono::microseconds elapsedTime)
	{

	}

	void MainScreen::AddEntity(entities::EntityPtr entity)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		newEntities.push_back(entity);
	}

	void MainScreen::RemoveEntity(entities::Entity::IdType id)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		removeEntities.insert(id);
	}

}