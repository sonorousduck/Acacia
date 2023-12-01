#include "Application.hpp"
#include "EntryPoint.hpp"
#include <graphics2d.hpp>
#include "screens/screen.hpp"
#include "screens/screenEnums.hpp"
#include "screens/gameScreen.hpp"
#include <singletons/time.hpp>
#include "../Audio/src/audioManager.hpp"

namespace Ebony {

	class BrickBreakerGame : public Application
	{
	public:
		BrickBreakerGame()
		{
	
		}

		~BrickBreakerGame()
		{
			graphics->Cleanup();
		}
			
		void Init() override
		{
			// Set up graphics here
			graphics = std::make_shared<Graphics2d>();
			graphics->Initialize("Brick Breaker", windowWidth, windowHeight);
			EbonyAudio::AudioManager::Init();

			// Add screens here as well
			screens[BrickBreaker::ScreenEnum::GAME] = std::make_shared<BrickBreaker::GameScreen>();

			currentScreen = screens[BrickBreaker::ScreenEnum::GAME];
			nextScreenEnum = BrickBreaker::ScreenEnum::GAME;
			// TODO: Get ResourceManager to register fonts in a good way, but for now, use the graphics.LoadFont way
			// Also, register any default fonts that I want to include throughout all the project

			for (auto& screen : screens)
			{
				screen.second->Init(graphics);
			}


			Application::Init();
		}

		void LoadContent() override
		{
			for (auto& screen : screens)
			{
				screen.second->LoadContent();
			}
		}

		void ProcessInput(std::chrono::microseconds elapsedTime) override
		{
			currentScreen->ProcessInput(elapsedTime);
		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			currentScreen->windowHeight = windowHeight;
			currentScreen->windowWidth = windowWidth;

			if (newScreenFocused)
			{
				currentScreen->OnScreenFocus();
				newScreenFocused = false;
			}

			nextScreenEnum = currentScreen->Update(elapsedTime);


			if (screens[nextScreenEnum] != currentScreen)
			{
				currentScreen->OnScreenDefocus();
				newScreenFocused = true;
			}
		

			Application::Update(elapsedTime);
		}


		void Draw(std::chrono::microseconds elapsedTime) override
		{
			graphics->BeginDraw(clearColor);
			currentScreen->Draw(elapsedTime);


			Application::Draw(elapsedTime);
			graphics->EndDraw();
		}
		
		void ChangeScreens() override
		{
			currentScreen = screens[nextScreenEnum];
		}

		void AddNewEntities() override
		{
			currentScreen->AddNewEntities();
		}

		void RemoveOldEntities() override
		{
			currentScreen->RemoveOldEntities();
		}


		void Run() override
		{
			Init();
			auto firstTime = std::chrono::system_clock::now();
			float currentFrame = static_cast<float>(glfwGetTime());
			auto previousTime = std::chrono::system_clock::now();

			while (true)
			{
				auto currentTime = std::chrono::system_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
				Ebony::Time::SetDeltaTime(elapsedTime);
				previousTime = currentTime;

				ProcessInput(elapsedTime);
				Update(elapsedTime);
				Draw(elapsedTime);

				RemoveOldEntities();
				AddNewEntities();
				glfwPollEvents();
				ChangeScreens();
			}
			
		}

	public:
		std::shared_ptr<Ebony::Graphics2d> graphics;

		int windowWidth = 800;
		int windowHeight = 600;

		bool newScreenFocused = false;

	private:
		std::shared_ptr<Screen> currentScreen;
		std::unordered_map<std::uint16_t, std::shared_ptr<Screen>> screens{};

		std::uint16_t nextScreenEnum = Ebony::ScreenEnum::DEFAULT;
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;

	};

	Ebony::Application* Ebony::CreateApplication()
	{
		return new BrickBreakerGame();
	}


}