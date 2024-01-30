#include "Application.hpp"
#include "EntryPoint.hpp"
#include <graphics2d.hpp>
#include "screens/screen.hpp"
#include "screens/screenEnums.hpp"
#include "screens/gameScreen.hpp"
#include <singletons/time.hpp>
#include <singletons/inputManager.hpp>
#include "../Ebony/src/singletons/audioManager.hpp"
#include "screens/mainScreen.hpp"
#include <singletons/systemManager.hpp>
#include <misc/actionSpaces.hpp>

#include <pybind11/pybind11.h>
#include "singletons/PythonManager.hpp"

namespace Ebony {

	class BrickBreakerGame : public Application
	{
	public:
		BrickBreakerGame(bool isAiControlled = false, bool isAiStartUp = false)
		{
			isAI = isAiControlled;
			isAIStartUp = isAiStartUp;
		}

		~BrickBreakerGame()
		{
			Ebony::Graphics2d::Cleanup();
		}
			
		void Init() override
		{
			// Set up graphics here
			Application::Init();

			Ebony::Graphics2d::Initialize("Brick Breaker", windowWidth, windowHeight, renderWidth, renderHeight);
			Ebony::AudioManager::Init();
			InputManager::Initialize();


			// Add screens here as well
			Ebony::SystemManager::screens[BrickBreaker::ScreenEnum::GAME] = std::make_shared<BrickBreaker::GameScreen>();
			Ebony::SystemManager::screens[BrickBreaker::ScreenEnum::MAIN_MENU] = std::make_shared<BrickBreaker::MainScreen>();
			Ebony::SystemManager::aiEnabled = isAI;

			if (isAI)
			{
				Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[BrickBreaker::ScreenEnum::GAME];
				Ebony::SystemManager::nextScreenEnum = BrickBreaker::ScreenEnum::GAME;
			}
			else
			{
				Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[BrickBreaker::ScreenEnum::MAIN_MENU];
				Ebony::SystemManager::nextScreenEnum = BrickBreaker::ScreenEnum::MAIN_MENU;
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
			//Ebony::Graphics2d::EndImgui();
			Ebony::Graphics2d::EndDraw();
		}
		
		void ChangeScreens() override
		{
			// Reset the screen to have the next screen of itself so it doesn't infinitely loop
			Ebony::SystemManager::currentScreen = Ebony::SystemManager::screens[Ebony::SystemManager::nextScreenEnum];
			Ebony::SystemManager::currentScreenEnum = Ebony::SystemManager::nextScreenEnum;
			if (Ebony::SystemManager::nextScreenEnum == BrickBreaker::QUIT)
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
				BrickBreaker::PythonManager::Init("pythonScripts.main", true);
			}

			else
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



				auto firstTime = std::chrono::system_clock::now();
				auto previousTime = std::chrono::system_clock::now();
				int frame = 0;

				Ebony::SystemManager::currentScreen->Start();

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
						auto currentTime = std::chrono::system_clock::now();
						auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
						Ebony::Time::SetDeltaTime(elapsedTime);
						previousTime = currentTime;
					}

					frame++;
				}

				ThreadPool::terminate();
				//EbonyAudio::AudioManager::StopAll();
				Ebony::AudioManager::StopAll();
				Ebony::ResourceManager::Clear();

				if (isAIStartUp)
				{
					BrickBreaker::PythonManager::Destroy();
				}

			}
		}

	public:

		int windowWidth = 1920 / 2;
		int windowHeight = 1080 / 2;
		int renderWidth = 480;
		int renderHeight = 320;

		bool isAI;
		bool isAIStartUp;


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
			//Crypt::CryptPythonManager::Init("pythonScripts.crypt_main");

			Ebony::SystemManager::currentScreen->Start();
		}

		std::tuple<BrickBreaker::State, Ebony::Discrete, bool, bool, std::unordered_map<std::string, std::string>> Step(pybind11::object action, int timestep)
		{
			BrickBreaker::PythonManager::action = action.cast<int>();;

			Ebony::Time::SetDeltaTime(std::chrono::microseconds(timestep));
			ProcessInput(std::chrono::microseconds(timestep));
			Update(std::chrono::microseconds(timestep));
			RemoveOldEntities();
			AddNewEntities();
			std::unordered_map<std::string, std::string> info = {};


			return std::make_tuple(BrickBreaker::PythonManager::state, BrickBreaker::PythonManager::reward, Ebony::SystemManager::shouldResetForAi, Ebony::SystemManager::shouldResetForAi, info);
		}

		BrickBreaker::State GetState()
		{
			return BrickBreaker::PythonManager::state;
		}

		Ebony::Discrete GetReward()
		{
			return BrickBreaker::PythonManager::reward;
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

		Ebony::Color clearColor = Ebony::Colors::Black;

	};

	Ebony::Application* Ebony::CreateApplication()
	{
		return new BrickBreakerGame(true, true);
	}


	PYBIND11_EMBEDDED_MODULE(brickbreaker, m)
	{
		pybind11::class_<BrickBreakerGame>(m, "BrickBreaker")
			.def(pybind11::init<bool, bool>())
			.def("Init", &BrickBreakerGame::Init)
			.def("LoadContent", &BrickBreakerGame::LoadContent)
			.def("ProcessInput", &BrickBreakerGame::ProcessInput)
			.def("Update", &BrickBreakerGame::Update)
			.def("Draw", &BrickBreakerGame::Draw)
			.def("ChangeScreens", &BrickBreakerGame::ChangeScreens)
			.def("step", &BrickBreakerGame::Step, pybind11::return_value_policy::move)
			.def("reset", &BrickBreakerGame::Reset)
			.def("render", &BrickBreakerGame::Render)
			.def("close", &BrickBreakerGame::Close)
			.def("getObservation", &BrickBreakerGame::GetState, pybind11::return_value_policy::move)
			.def("getReward", &BrickBreakerGame::GetReward, pybind11::return_value_policy::move)
			.def("python_init", &BrickBreakerGame::PythonInit)
			.def("getTerminated", &BrickBreakerGame::GetTerminated);
	}


}