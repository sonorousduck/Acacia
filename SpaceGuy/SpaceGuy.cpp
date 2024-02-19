#include "Application.hpp"
#include "EntryPoint.hpp"
#include <graphics2d.hpp>
#include "screens/screen.hpp"
#include "screens/screenEnums.hpp"
#include <singletons/time.hpp>
#include <singletons/inputManager.hpp>
#include <singletons/audioManager.hpp>
#include <singletons/systemManager.hpp>
#include "singletons/SpaceGuyPythonManager.hpp"
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

	class SpaceGuyGame : public Application
	{
	public:
		SpaceGuyGame(bool isAiControlled = false, bool isAiStartUp = false)
		{
			isAI = isAiControlled;
			isAIStartUp = isAiStartUp;
		}

		~SpaceGuyGame()
		{
			Ebony::Graphics2d::Cleanup();
		}
			
		void Init() override
		{
			// Set up graphics here
			Application::Init();



			Ebony::Graphics2d::Initialize("SpaceGuy", windowWidth, windowHeight, renderWidth, renderHeight);
			Ebony::AudioManager::Init();
			InputManager::Initialize();
			//SpaceGuy::SpaceGuyPythonManager::Init("pythonScripts.SpaceGuy_main");
			//SDL_SetWindowMouseGrab(Ebony::Graphics2d::window.getWindow(), SDL_TRUE);

			// Add screens here as well

			Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::MAIN_MENU] = std::make_shared<SpaceGuy::MainMenuScreen>();
			Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::GAME] = std::make_shared<SpaceGuy::MainGameScreen>();
			Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::CONTROLS] = std::make_shared<SpaceGuy::ControlsScreen>();
			Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::HIGH_SCORE] = std::make_shared<SpaceGuy::HighScoreScreen>();
			Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::OPTIONS] = std::make_shared<SpaceGuy::OptionsMenuScreen>();
			Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::PAUSE] = std::make_shared<SpaceGuy::PauseScreen>();
			Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::GAME_OVER] = std::make_shared<SpaceGuy::GameOverScreen>();


			Ebony::SystemManager::aiEnabled = isAI;

			if (isAI)
			{
				Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::GAME];
				Ebony::SystemManager::nextScreenEnum = SpaceGuy::ScreenEnum::GAME;
			}
			else
			{
				Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[SpaceGuy::ScreenEnum::MAIN_MENU];
				Ebony::SystemManager::nextScreenEnum = SpaceGuy::ScreenEnum::MAIN_MENU;
			}



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
			if (Ebony::SystemManager::nextScreenEnum == SpaceGuy::QUIT)
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
			if (isAIStartUp)
			{
				SpaceGuy::SpaceGuyPythonManager::Init("pythonScripts.main", true);
				return;
			}


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

			if (isAIStartUp)
			{
				SpaceGuy::SpaceGuyPythonManager::Destroy();
			}
		}
		
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


		std::tuple<SpaceGuy::State, Ebony::Discrete, bool, bool, std::unordered_map<std::string, std::string>> Step(pybind11::object action, int timestep)
		{
			SpaceGuy::SpaceGuyPythonManager::action = action.cast<Ebony::Action>();

			Ebony::Time::SetDeltaTime(std::chrono::microseconds(timestep));
			ProcessInput(std::chrono::microseconds(timestep));
			Update(std::chrono::microseconds(timestep));
			RemoveOldEntities();
			AddNewEntities();
			std::unordered_map<std::string, std::string> info = {};


			return std::make_tuple(SpaceGuy::SpaceGuyPythonManager::state, SpaceGuy::SpaceGuyPythonManager::reward, Ebony::SystemManager::shouldResetForAi, Ebony::SystemManager::shouldResetForAi, info);
		}

		SpaceGuy::State GetState()
		{
			return SpaceGuy::SpaceGuyPythonManager::state;
		}

		Ebony::Discrete GetReward()
		{
			return SpaceGuy::SpaceGuyPythonManager::reward;
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

	
	public:

		int windowWidth = 640;
		int windowHeight = 480;
		int renderWidth = 480;
		int renderHeight = 320;

		bool isAI;
		bool isAIStartUp;

	private:
		Ebony::Color clearColor = Ebony::Colors::Black;

	};

	Ebony::Application* Ebony::CreateApplication()
	{
		return new SpaceGuyGame(false, false);
	}

	PYBIND11_EMBEDDED_MODULE(spaceguy, m)
	{
		pybind11::class_<SpaceGuyGame>(m, "SpaceGuy")
			.def(pybind11::init<bool, bool>())
			.def("Init", &SpaceGuyGame::Init)
			.def("LoadContent", &SpaceGuyGame::LoadContent)
			.def("ProcessInput", &SpaceGuyGame::ProcessInput)
			.def("Update", &SpaceGuyGame::Update)
			.def("Draw", &SpaceGuyGame::Draw)
			.def("ChangeScreens", &SpaceGuyGame::ChangeScreens)
			.def("step", &SpaceGuyGame::Step, pybind11::return_value_policy::move)
			.def("reset", &SpaceGuyGame::Reset)
			.def("render", &SpaceGuyGame::Render)
			.def("close", &SpaceGuyGame::Close)
			.def("getObservation", &SpaceGuyGame::GetState, pybind11::return_value_policy::move)
			.def("getReward", &SpaceGuyGame::GetReward, pybind11::return_value_policy::move)
			.def("python_init", &SpaceGuyGame::PythonInit)
			.def("getTerminated", &SpaceGuyGame::GetTerminated);
	}
}