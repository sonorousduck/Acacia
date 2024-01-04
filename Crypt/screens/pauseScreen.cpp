#include "pauseScreen.hpp"
#include "../prefabs/UI/buttonTextPrefab.hpp"
#include "../prefabs/UI/buttonPrefab.hpp"
#include "../prefabs/menuCursorPrefab.hpp"

namespace Crypt
{

	void PauseScreen::LoadContent()
	{
		Ebony::ResourceManager::LoadTexture("Button_Unpressed.tx", "button_unpressed", "Crypt");
		Ebony::ResourceManager::LoadTexture("Button_Pressed.tx", "button_pressed", "Crypt");
		Ebony::ResourceManager::LoadTexture("Button_Hovered.tx", "button_hovered", "Crypt");
		Ebony::ResourceManager::LoadTexture("Resume_Text.tx", "resume_text", "Crypt");
		Ebony::ResourceManager::LoadTexture("Quit_Text.tx", "quit_text", "Crypt");
	}

	void PauseScreen::Start()
	{
		std::shared_ptr<Shader> s = Ebony::ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
		Ebony::ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");

		Ebony::Graphics2d::InitializeTextDrawing(Ebony::ResourceManager::GetShader("text"));


		s->use();
		s->setInt("image", 0);
		s->setMat4("projection", Ebony::Graphics2d::projection);

		physicsSystem = systems::PhysicsSystem();
		spriteRenderer = systems::SpriteRenderer();

		spriteRenderer.debug = false;

		inputSystem = systems::InputSystem();
		audioSystem = systems::AudioSystem();

		// Create prefabs
		AddEntity(Crypt::MenuCursor::Create());


		auto button = Crypt::Button::Create(0.0f, 140.0f, static_cast<float>(windowWidth), "button_unpressed", "button_hovered", "button_pressed", Crypt::ScreenEnum::GAME, [=](std::uint16_t nextScreen) {SetNextScreen(nextScreen); });
		auto actualButtonScale = button->getComponent<components::Transform>()->scale;
		auto buttonPositioningX = button->getComponent<components::Transform>()->position.x;

		AddEntity(button);

		AddEntity(Crypt::ButtonText::Create(buttonPositioningX, 140.0f, actualButtonScale.x, actualButtonScale.y, "resume_text"));

		

		AddEntity(Crypt::Button::Create(0.0f, 150.0f + actualButtonScale.y, static_cast<float>(windowWidth), "button_unpressed", "button_hovered", "button_pressed", Crypt::ScreenEnum::QUIT, [=](std::uint16_t nextScreen) {SetNextScreen(nextScreen); }));
		AddEntity(Crypt::ButtonText::Create(buttonPositioningX, 150.0f + actualButtonScale.y, actualButtonScale.x, actualButtonScale.y, "quit_text"));


		AddNewEntities();
		alreadyStarted = true;
	}

	void PauseScreen::Init(int windowWidth, int windowHeight)
	{
		camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));

		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
		mainRenderTarget = Ebony::RenderTarget2D::Create(windowWidth, windowHeight, GL_LINEAR, GL_NEAREST);
		clearColor = Ebony::Colors::Black;

		Ebony::Graphics2d::SetMainCamera(camera);
	}

	

	std::uint64_t PauseScreen::Update(std::chrono::microseconds elapsedTime)
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

		auto inputTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				inputSystem.Update();
			}
		);


		Ebony::ThreadPool::instance().submitTaskGraph(taskGraph);
		graphDone.wait();

		return nextScreen;

	}

	void PauseScreen::Draw(std::chrono::microseconds elapsedTime)
	{
		Ebony::Graphics2d::SetRenderTarget(mainRenderTarget, clearColor);

		// Draw things!
		spriteRenderer.Update();

		Ebony::Graphics2d::UnbindRenderTarget(clearColor);

		Ebony::Graphics2d::DrawRenderTarget(Ebony::ResourceManager::GetShader("screenTexture"), mainRenderTarget);
	}

	void PauseScreen::ProcessInput(std::chrono::microseconds elapsedTime)
	{

	}

	void PauseScreen::AddEntity(entities::EntityPtr entity)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		newEntities.push_back(entity);
	}

	void PauseScreen::RemoveEntity(entities::Entity::IdType id)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		removeEntities.insert(id);
	}

	void PauseScreen::OnScreenDefocus(std::uint64_t nextScreenEnum)
	{
		nextScreen = screen;

		
	}

	void PauseScreen::OnScreenFocus(std::uint64_t lastScreenEnum)
	{
		if (!alreadyStarted)
		{
			Start();
		}
		Ebony::Graphics2d::SetMainCamera(camera);
	}

    void PauseScreen::SetNextScreen(std::uint16_t nextScreenIncoming)
	{
		nextScreen = nextScreenIncoming;
	}


	void PauseScreen::AddNewEntities()
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

	void PauseScreen::RemoveOldEntities()
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

}