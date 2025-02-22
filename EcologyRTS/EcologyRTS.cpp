#include "Application.hpp"
#include "EntryPoint.hpp"
#include <graphics2d.hpp>
#include "screens/screen.hpp"
#include "screens/screenEnums.hpp"
#include <singletons/time.hpp>
#include <singletons/inputManager.hpp>
#include <singletons/audioManager.hpp>
#include <singletons/systemManager.hpp>
#include "singletons/EcologyPythonManager.hpp"
#include <misc/resourceManager.hpp>
#include "misc/ThreadPool.hpp"
#include "screens/screenEnums.hpp"

#include "screens/mainMenuScreen.hpp"
#include "screens/mainGameScreen.hpp"
#include "screens/controlsScreen.hpp"
#include "screens/highScoreScreen.hpp"
#include "screens/optionsMenuScreen.hpp"
#include "screens/pauseScreen.hpp"
#include "screens/gameOverScreen.hpp"

#include <pybind11/pybind11.h>

namespace Ebony {

	class EcologyRTSGame : public Application
	{
	public:
		EcologyRTSGame()
		{
			//pybind11::scoped_interpreter guard{};

		}

		~EcologyRTSGame()
		{
			Ebony::Graphics2d::Cleanup();
		}
			
		void Init() override
		{
			// Set up graphics here
			Application::Init();



			Ebony::Graphics2d::Initialize("EcologyRTS", windowWidth, windowHeight, renderWidth, renderHeight);
			Ebony::AudioManager::Init();
			InputManager::Initialize();
			//EcologyRTS::EcologyRTSPythonManager::Init("pythonScripts.EcologyRTS_main");
			//SDL_SetWindowMouseGrab(Ebony::Graphics2d::window.getWindow(), SDL_TRUE);

			// Add screens here as well

			Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::MAIN_MENU] = std::make_shared<EcologyRTS::MainMenuScreen>();
			Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::GAME] = std::make_shared<EcologyRTS::MainGameScreen>();
			Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::CONTROLS] = std::make_shared<EcologyRTS::ControlsScreen>();
			Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::HIGH_SCORE] = std::make_shared<EcologyRTS::HighScoreScreen>();
			Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::OPTIONS] = std::make_shared<EcologyRTS::OptionsMenuScreen>();
			Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::PAUSE] = std::make_shared<EcologyRTS::PauseScreen>();
			Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::GAME_OVER] = std::make_shared<EcologyRTS::GameOverScreen>();


			Ebony::SystemManager::aiEnabled = true;

			Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[EcologyRTS::ScreenEnum::MAIN_MENU];
			Ebony::SystemManager::nextScreenEnum = EcologyRTS::ScreenEnum::MAIN_MENU;

			for (auto& screen : Ebony::SystemManager::screens)
			{
				screen.second->Init(renderWidth, renderHeight);
			}
		}

		void LoadContent() override
		{
			Ebony::SystemManager::currentScreen->LoadContent();

			Application::LoadContent();
		}

		void ProcessInput(std::chrono::microseconds elapsedTime) override
		{
			// Update the SDL information here
			Ebony::SystemManager::quit = InputManager::HandleInput();

			Ebony::SystemManager::currentScreen->ProcessInput(elapsedTime);

			Application::ProcessInput(elapsedTime);
		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			Ebony::SystemManager::currentScreen->windowHeight = windowHeight;
			Ebony::SystemManager::currentScreen->windowWidth = windowWidth;

			if (Ebony::SystemManager::newScreenFocused)
			{
				Ebony::SystemManager::currentScreen->OnScreenFocus(Ebony::SystemManager::lastScreenEnum);
				Ebony::SystemManager::newScreenFocused = false;
			}

			std::uint64_t toGetRidOfEventually = Ebony::SystemManager::currentScreen->Update(elapsedTime);


			if (Ebony::SystemManager::screens[Ebony::SystemManager::nextScreenEnum] != Ebony::SystemManager::currentScreen)
			{
				Ebony::SystemManager::lastScreenEnum = Ebony::SystemManager::currentScreenEnum;
				Ebony::SystemManager::currentScreen->OnScreenDefocus(Ebony::SystemManager::nextScreenEnum);
				Ebony::SystemManager::newScreenFocused = true;
			}
		

			Application::Update(elapsedTime);
		}


		void Draw(std::chrono::microseconds elapsedTime) override
		{
			Ebony::Graphics2d::BeginDraw(clearColor);
			Ebony::SystemManager::currentScreen->Draw(elapsedTime);
			Ebony::Graphics2d::DrawFromQueue();

			Application::Draw(elapsedTime);
			Ebony::Graphics2d::EndDraw();
		}
		
		void ChangeScreens() override
		{
			// Reset the screen to have the next screen of itself so it doesn't infinitely loop
			Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[Ebony::SystemManager::nextScreenEnum];
			Ebony::SystemManager::currentScreenEnum = Ebony::SystemManager::nextScreenEnum;
			if (Ebony::SystemManager::nextScreenEnum == EcologyRTS::QUIT)
			{
				Ebony::SystemManager::quit = true;
			}
			else
			{
				Ebony::SystemManager::currentScreen->LoadContent();
			}
		}

		void AddNewEntities() override
		{
			Ebony::SystemManager::currentScreen->AddNewEntities();
		}

		void RemoveOldEntities() override
		{
			Ebony::SystemManager::currentScreen->RemoveOldEntities();
		}


		void Run() override
		{
			Init();
			LoadContent();

			std::shared_ptr<Shader> s1 = Ebony::ResourceManager::LoadShader("shaders/screenTexture.vert", "shaders/screenTexture.frag", "screenTexture");

			s1->use();
			s1->setInt("screenTexture", 0);


			std::shared_ptr<Shader> s = Ebony::ResourceManager::LoadShader("shaders/spritesheet3d.vert", "shaders/spritesheet3d.frag", "spritesheet");

			s->use();
			s->setInt("spritesheet", 0);
			s->setMat4("projection", Ebony::Graphics2d::projection);

			std::shared_ptr<Shader> s2 = Ebony::ResourceManager::LoadShader("shaders/particle.vert", "shaders/particle.frag", "defaultParticle");

			s2->use();
			s2->setInt("particleTexture", 0);
			s2->setMat4("projection", Ebony::Graphics2d::projection);


			Ebony::SystemManager::currentScreen->Start();
			auto firstTime = std::chrono::system_clock::now();
			auto previousTime = std::chrono::system_clock::now();
			int frame = 0;

			while (!Ebony::SystemManager::quit)
			{
				auto currentTime = std::chrono::system_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
				Ebony::Time::SetDeltaTime(elapsedTime);
				previousTime = currentTime;

				Ebony::InputManager::ResetInput();

				//EB_TRACE(frame);
				ProcessInput(elapsedTime);
				Update(elapsedTime);
				Draw(elapsedTime);

				RemoveOldEntities();
				AddNewEntities();

				if (Ebony::SystemManager::newScreenFocused)
				{
					ChangeScreens();
					// Need to reset elapsed time after changing screens so the elapsed time isn't counted when loading information and so forth
					auto currentTime = std::chrono::system_clock::now();
					auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
					Ebony::Time::SetDeltaTime(elapsedTime);
					previousTime = currentTime;
				}

				frame++;
			}
			
			ThreadPool::terminate();
			Ebony::AudioManager::StopAll();
			Ebony::ResourceManager::Clear();
			EcologyRTS::EcologyPythonManager::Destroy();
		}

	public:

		int windowWidth = 960;
		int windowHeight = 640;
		int renderWidth = 480;
		int renderHeight = 320;

		bool isAI = false;

	private:
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;

	};

	Ebony::Application* Ebony::CreateApplication()
	{
		return new EcologyRTSGame();
	}


}