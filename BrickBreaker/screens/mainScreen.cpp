#include "mainScreen.hpp"

#include "../prefabs/buttonTextPrefab.hpp"
#include "../prefabs/logoPrefab.hpp"
#include "../prefabs/buttonPrefab.hpp"
#include "../prefabs/menuCursorPrefab.hpp"

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

	void MainScreen::Start()
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

		auto button = BrickBreaker::Button::Create(0.0f, 120.0f, static_cast<float>(windowWidth), "button_unpressed", "button_hovered", "button_pressed", BrickBreaker::ScreenEnum::GAME);
		auto actualButtonScale = button->getComponent<components::Transform>()->scale;
		auto buttonPositioningX = button->getComponent<components::Transform>()->position.x;

		AddEntity(BrickBreaker::MenuCursor::Create());


		AddEntity(BrickBreaker::Logo::Create(0.0f, 0.0f, "logo_brickbreaker"));
		AddEntity(button);

		AddEntity(BrickBreaker::ButtonText::Create(buttonPositioningX, 120.0f, actualButtonScale.x, actualButtonScale.y, "start_text"));
		AddEntity(BrickBreaker::Button::Create(0.0f, 130.0f + actualButtonScale.y, static_cast<float>(windowWidth), "button_unpressed", "button_hovered", "button_pressed", BrickBreaker::ScreenEnum::OPTIONS));
		AddEntity(BrickBreaker::ButtonText::Create(buttonPositioningX, 130.0f + actualButtonScale.y, actualButtonScale.x, actualButtonScale.y, "options_text"));
		AddEntity(BrickBreaker::Button::Create(0.0f, 140.0f + 2 * actualButtonScale.y, static_cast<float>(windowWidth), "button_unpressed", "button_hovered", "button_pressed", BrickBreaker::ScreenEnum::QUIT));
		AddEntity(BrickBreaker::ButtonText::Create(buttonPositioningX, 140.0f + 2.0f * actualButtonScale.y, actualButtonScale.x, actualButtonScale.y, "quit_text"));

		AddNewEntities();
	}

	void MainScreen::Init(int windowWidth, int windowHeight)
	{
		camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));

		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
		mainRenderTarget = Ebony::RenderTarget2D::Create(windowWidth, windowHeight, GL_LINEAR, GL_NEAREST);
		clearColor = Ebony::Colors::Black;

		Ebony::Graphics2d::SetMainCamera(camera);

	}

	void MainScreen::SetNextScreen(std::uint16_t nextScreenIncoming)
	{
		nextScreen = nextScreenIncoming;
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

	std::uint64_t MainScreen::Update(std::chrono::microseconds elapsedTime)
	{
		auto firstTime = std::chrono::system_clock::now();

		std::latch graphDone{ 1 };

		auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
			[&graphDone]()
			{
				graphDone.count_down();
			});

		// UI will need physics layer, input system, music, sprite

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

		physicsSystem.Update(elapsedTime);


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

	void MainScreen::OnScreenDefocus(std::uint64_t nextScreenEnum)
	{
		nextScreen = screen;

		std::cout << "Defocusing Main Screen" << std::endl;
	}

	void MainScreen::OnScreenFocus(std::uint64_t lastScreenEnum)
	{
		Start();

		std::cout << "Focusing Main Screen" << std::endl;
	}

}