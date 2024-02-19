#include "Application.hpp"
#include "EntryPoint.hpp"
#include <graphics2d.hpp>
#include "screens/screen.hpp"
#include "screens/screenEnums.hpp"
#include <singletons/time.hpp>
#include <singletons/inputManager.hpp>
#include <singletons/audioManager.hpp>
#include <singletons/systemManager.hpp>
#include "singletons/CryptPythonManager.hpp"
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

	class CryptGame : public Application
	{
	public:
		CryptGame(bool isAiControlled = false, bool isAiStartUp = false)
		{
			isAI = isAiControlled;
			isAIStartUp = isAiStartUp;
		}

		~CryptGame()
		{
			Ebony::Graphics2d::Cleanup();
		}
			
		void Init() override
		{
			// Set up graphics here
			Application::Init();

			Ebony::Graphics2d::Initialize("Crypt", windowWidth, windowHeight, renderWidth, renderHeight);
			Ebony::AudioManager::Init();
			InputManager::Initialize();


			// Add screens here as well

			Ebony::SystemManager::screens[Crypt::ScreenEnum::MAIN_MENU] = std::make_shared<Crypt::MainMenuScreen>();
			Ebony::SystemManager::screens[Crypt::ScreenEnum::GAME] = std::make_shared<Crypt::MainGameScreen>();
			Ebony::SystemManager::screens[Crypt::ScreenEnum::CONTROLS] = std::make_shared<Crypt::ControlsScreen>();
			Ebony::SystemManager::screens[Crypt::ScreenEnum::HIGH_SCORE] = std::make_shared<Crypt::HighScoreScreen>();
			Ebony::SystemManager::screens[Crypt::ScreenEnum::OPTIONS] = std::make_shared<Crypt::OptionsMenuScreen>();
			Ebony::SystemManager::screens[Crypt::ScreenEnum::PAUSE] = std::make_shared<Crypt::PauseScreen>();
			Ebony::SystemManager::screens[Crypt::ScreenEnum::GAME_OVER] = std::make_shared<Crypt::GameOverScreen>();


			Ebony::SystemManager::aiEnabled = isAI;

			if (isAI)
			{

				Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[Crypt::ScreenEnum::GAME];
				Ebony::SystemManager::nextScreenEnum = Crypt::ScreenEnum::GAME;
			}
			else
			{
				Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[Crypt::ScreenEnum::MAIN_MENU];
				Ebony::SystemManager::nextScreenEnum = Crypt::ScreenEnum::MAIN_MENU;
			}


			// TODO: Get ResourceManager to register fonts in a good way, but for now, use the graphics.LoadFont way
			// Also, register any default fonts that I want to include throughout all the project

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
			//Ebony::Graphics2d::EndImgui();
			Ebony::Graphics2d::EndDraw();
		}
		
		void ChangeScreens() override
		{
			// Reset the screen to have the next screen of itself so it doesn't infinitely loop
			Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[Ebony::SystemManager::nextScreenEnum];
			Ebony::SystemManager::currentScreenEnum = Ebony::SystemManager::nextScreenEnum;
			if (Ebony::SystemManager::nextScreenEnum == Crypt::QUIT)
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
			// If is AI startup, we will actually skip all of these steps. This will just make it so we start the C++ and it starts up the Python script that will generate
			// more instances of the engine. 

			if (isAIStartUp)
			{
				Crypt::CryptPythonManager::Init("pythonScripts.main", true);
				return;
			}
			else
			{
				Init();
				LoadContent();
				//Crypt::CryptPythonManager::Init("pythonScripts.crypt_main");

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

				Ebony::ThreadPool::terminate();
				Ebony::AudioManager::StopAll();
				Ebony::ResourceManager::Clear();

				if (isAIStartUp)
				{
					Crypt::CryptPythonManager::Destroy();
				}
			}
		}

	public:

		int windowWidth = 640;
		int windowHeight = 480;
		int renderWidth = 480;
		int renderHeight = 320;

		bool isAI;
		bool isAIStartUp;
		//bool quit = false;

		//bool newScreenFocused = false;


	public:
		// Gymnasium compliant functions that are passed to the python script

		void PythonInit(bool renderingEnabled, bool restarting)
		{
			if (restarting)
			{
				Ebony::SystemManager::currentScreen->Start();
				Ebony::SystemManager::shouldResetForAi = false;


				return;
			}

			if (renderingEnabled)
			{
				Init();

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
			}
			LoadContent();

			Ebony::SystemManager::currentScreen->Start();
		}

		std::tuple<Crypt::State, Ebony::Discrete, bool, bool, std::unordered_map<std::string, std::string>> Step(pybind11::object action, int timestep)
		{
			Crypt::CryptPythonManager::action = action.cast<Ebony::Box>();

			Ebony::Time::SetDeltaTime(std::chrono::microseconds(timestep));
			ProcessInput(std::chrono::microseconds(timestep));
			Update(std::chrono::microseconds(timestep));
			RemoveOldEntities();
			AddNewEntities();
			std::unordered_map<std::string, std::string> info = {};


			return std::make_tuple(Crypt::CryptPythonManager::state, Crypt::CryptPythonManager::reward, Ebony::SystemManager::shouldResetForAi, Ebony::SystemManager::shouldResetForAi, info);
		}

		Crypt::State GetState()
		{
			return Crypt::CryptPythonManager::state;
		}

		Ebony::Discrete GetReward()
		{
			return Crypt::CryptPythonManager::reward;
		}

		bool GetTerminated()
		{
			return Ebony::SystemManager::shouldResetForAi;
		}

		void Reset()
		{
			Ebony::SystemManager::currentScreen->RemoveAllEntities();
		}

		void Render(int timestep)
		{
			Draw(std::chrono::microseconds(timestep));
		}

		void Close()
		{

		}


	private:
		//std::shared_ptr<Screen> currentScreen;
		//std::unordered_map<std::uint64_t, std::shared_ptr<Screen>> screens{};

		//std::uint64_t nextScreenEnum = Ebony::ScreenEnum::DEFAULT;
		//std::uint64_t currentScreenEnum = Ebony::ScreenEnum::DEFAULT;
		//std::uint64_t lastScreenEnum = Ebony::ScreenEnum::DEFAULT;

		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;

	};

	Ebony::Application* Ebony::CreateApplication()
	{
		return new CryptGame(false, false);
	}


	PYBIND11_EMBEDDED_MODULE(crypt, m)
	{
		pybind11::class_<CryptGame>(m, "Crypt")
			.def(pybind11::init<bool, bool>())
			.def("Init", &CryptGame::Init)
			.def("LoadContent", &CryptGame::LoadContent)
			.def("ProcessInput", &CryptGame::ProcessInput)
			.def("Update", &CryptGame::Update)
			.def("Draw", &CryptGame::Draw)
			.def("ChangeScreens", &CryptGame::ChangeScreens)
			.def("step", &CryptGame::Step, pybind11::return_value_policy::move)
			.def("reset", &CryptGame::Reset)
			.def("render", &CryptGame::Render)
			.def("close", &CryptGame::Close)
			.def("getObservation", &CryptGame::GetState, pybind11::return_value_policy::move)
			.def("getReward", &CryptGame::GetReward, pybind11::return_value_policy::move)
			.def("python_init", &CryptGame::PythonInit)
			.def("getTerminated", &CryptGame::GetTerminated);
	}


}